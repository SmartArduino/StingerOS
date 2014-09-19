#include "stinger.h"
#include "hal.h"
#include "msg.h"
#include "user.h"
#include "timer.h"
#include "cpu.h"
#include "includes.h"

struct pcb RunningProcList, WaitingProcList;
struct pcb *current=NULL, *idle = NULL, *next;
char gTag=0x01;

void insert_pcb_to_list_tail(process_control_block_t *pHead, process_control_block_t **pNewPCB){
    (*pNewPCB)->next=pHead;
    (*pNewPCB)->prev=pHead->prev;
    pHead->prev->next=*pNewPCB;
    pHead->prev=*pNewPCB;
}

void insert_pcb_to_list_head(process_control_block_t *pHead, process_control_block_t **pNewPCB){
	(*pNewPCB)->next=pHead->next;
	(*pNewPCB)->prev=pHead;
	pHead->next->prev=*pNewPCB;
	pHead->next=*pNewPCB;
}

UINT8 delete_pcb_from_list(process_control_block_t *head, process_control_block_t *target){
	process_control_block_t *temp;

	temp = head->next;
	while(temp != head){
		if(temp->pid == target->pid){
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			target->next = NULL;
			target->prev = NULL;
			return 1;
		}
		temp = temp->next;
	}
	//printf("There is no such a pcb in the list!\n");
	return 0;
}

UINT8 is_list_empty(process_control_block_t *head){
	if(head->next == head){
		return 1;
	}else{
		return 0;
	}
}

UINT8 is_there_only_init_proc(void){
	if((RunningProcList.next)->next == &RunningProcList){
		return 1;
	}else{
		return 0;
	}
}

process_control_block_t *find_pcb(UINT8 pid){
	process_control_block_t *temp;

	temp = RunningProcList.next;
	while(temp != &RunningProcList){
		if(temp->pid == pid){
			return temp;
		}
		temp = temp->next;
	}

	temp = WaitingProcList.next;
	while(temp != &WaitingProcList){
		if(temp->pid == pid){
			return temp;
		}
		temp = temp->next;
	}

	return NULL;
}

void free_list_mem(process_control_block_t *head){
	process_control_block_t *temp;

	while(!is_list_empty(head)){
		temp = head->prev;

		if(temp->queue){	//Free message queue for each process
			os_mem_free(temp->queue);
		}

		temp->prev->next = head;
		head->prev = temp->prev;
		if(temp->prev == head){
			head->next = head;
		}
		os_mem_free(temp);
	}
	os_mem_free(head);
	//printf("Free memory!\n");
}

void process_wait(process_control_block_t *target){
	target->flag = SET_PROC_WAITING(target->flag);
	target->flag = SET_PROC_WAIT_MSG(target->flag);
	if(target != idle){
		if(is_there_only_init_proc()){
			switch_context(idle);
		}else{
			switch_context(idle->next);
		}
	}

	enter_critical_section();
	delete_pcb_from_list(&RunningProcList, target);
	insert_pcb_to_list_tail(&WaitingProcList, &target);
	exit_critical_section();/**/
}

UINT8 process_create(UINT8 pid, UINT8 prio, 
					 process_control_block_t *pNew, 
					 void (*proc)(void), 
					 UINT32 *pStack){
	struct pcb *temp;
	UINT32 /**psp,*/ parg=0;
	
	//Check pid in RunningProcList
	if(is_list_empty(&RunningProcList)){
		//There is no user process.
	}else{
		temp = RunningProcList.next;
		while(temp != &RunningProcList){
			if(temp->pid == pid){
				//printf("There is a process whose pid same as the given pid in RunningProcList!\n");
				return 0;
			}
			temp = temp->next;
		}
	}
	//Check pid in WaitingProcList
	if(is_list_empty(&WaitingProcList)){
		//There is no user process.
	}else{
		temp = WaitingProcList.next;
		while(temp != &WaitingProcList){
			if(temp->pid == pid){
				//printf("There is a process whose pid same as the given pid in WaitingProcList!\n");
				return 0;
			}
			temp = temp->next;
		}
	}

	pNew = (process_control_block_t *)os_mem_alloc((UINT8)sizeof(process_control_block_t));
	if(pNew){
		//printf("user process alloc success!\n");
	}else{
		return 0;
	}

	task_stack_init(proc, &parg, get_stack_end(pStack, PROC_STACK_SIZE/4));

	pNew->ptos		= pStack;
	pNew->pid		= pid;
	pNew->prio		= prio;
	pNew->slice		= DEFAULT_SLICE;
	pNew->status	= WAITING;
	pNew->flag		= 0x01;
	pNew->proc		= proc;
	pNew->timeout	= 0;

	process_msg_queue_init(pNew);

	enter_critical_section();
	insert_pcb_to_list_tail(&RunningProcList, &pNew);
	exit_critical_section();

	return 1;
}

void process_idle(void){
	UARTprintf("idle process is running!\n");
	if(IS_IDLE_FIRST_RUN(gTag)){
		gTag=SET_IDLE_RUNNING(gTag);

	}
}

void process_init(UINT32 *stackForIdle){

	RunningProcList.next = &RunningProcList;
	RunningProcList.prev = &RunningProcList;
	WaitingProcList.next = &WaitingProcList;
	WaitingProcList.prev = &WaitingProcList;

	process_create(0, 255, idle, process_idle, stackForIdle);
	//process_idle();
	idle = RunningProcList.next;
	current=idle;
	next=idle;
}

UINT32 *get_stack_end(UINT32 *pStk, UINT32 size){
	if(!pStk){
		return NULL;
	}
	return pStk+size-1;
}

int need_schedule(void){
	if(current != next){
		return 1;
	}
	return 0;
}

void os_schedule(void){
	process_control_block_t *temp, *temp2;
	/**
	 ** There are more than one process that need to run.
	 **/
	if(!is_there_only_init_proc()){
		if(!current){						//initial status
			next = idle;
		}else{								//schedule task and run the current task.
			if((current->slice != 0) && (current->flag & NEED_SCHEDULE)){
				(*(current->proc))();
				return;
			}
		}

		temp = (RunningProcList.next)->next;
		while(temp != &RunningProcList){
			if(temp->flag & NEED_SCHEDULE){	//Find next process to run.
				if(current->slice == 0){	//switch current process, when slice is zero.
					if(current != idle){	//Don't move idle process to WaitingProcList.
						current->slice=DEFAULT_SLICE;
									
						enter_critical_section();
						delete_pcb_from_list(&RunningProcList, current);
						insert_pcb_to_list_tail(&WaitingProcList, &current);
						exit_critical_section();
	
						switch_context(temp);
						break;
					}else{
						current->slice=DEFAULT_SLICE;
						switch_context(temp);
						break;
					}
				}
				//printf("pid=%d is set to current!\n", temp->pid);
			}else{							//This process neet to move to WaitingProcList
				temp2 = temp->next;			//save pointer

				enter_critical_section();
				delete_pcb_from_list(&RunningProcList, temp);
				insert_pcb_to_list_tail(&WaitingProcList, &temp);
				exit_critical_section();

				//printf("pid=%d delete from running list!\n", temp->pid);
				temp->slice = DEFAULT_SLICE;
				temp = temp2;				//get original pointer
				//Since current task has been deleted from list, so schedule next task
				//current = temp->next;
				continue;
			}
			temp = temp->next;
		}
	}else if(is_there_only_init_proc() && !is_list_empty(&WaitingProcList)){
		/**	
		 ** There is no process in RunningProcList, so we 
		 ** have to get some processes from WaitingProcList.
		 **/
		if(!is_list_empty(&WaitingProcList)){
			temp = WaitingProcList.next;
			while(temp != &WaitingProcList){
				/** 
				 **	We should check whether the process is waiting for message, if so, we
				 **	should leave it unchange.
				 **/
				if(!IS_PROC_WAIT_FOR_MSG(temp->flag)){
					//Insert to running process list and sort according to prio of process.
					temp->flag = SET_PROC_RUNNING(temp->flag);
					temp2 = temp->next;

					enter_critical_section();	
					delete_pcb_from_list(&WaitingProcList, temp);
					insert_pcb_to_list_tail(&RunningProcList, &temp);	//We should sort these processes!!!
					exit_critical_section();
				
					temp = temp2;
					continue;
				}
				temp = temp->next;
			}
			//current = (RunningProcList.next)->next;
			switch_context((RunningProcList.next)->next);
		}
	}else{
		/**
		 ** Run idle process. There is only idle process that can run.
		 **/
		if(idle->slice == 0){
			idle->slice = DEFAULT_SLICE;
			idle->flag = SET_PROC_RUNNING(idle->flag);
		}
		current = idle;
	}
}

