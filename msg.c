#include "msg.h"
#include "hal.h"

msg_t *MsgQueue = NULL, *MsgList = NULL;

UINT8 is_msg_list_empty(msg_t *head){
	if(head->next == head){
		return 1;
	}else{
		return 0;
	}
}

void insert_msg_to_queue(msg_t *head, msg_t *new){
	if(is_msg_list_empty(head)){
		head->next = new;
		head->prev = new;
		new->next = head;
		new->prev = head;
	}else{
		head->prev->next = new;
		new->prev = head->prev;
		new->next = head;
		head->prev = new;
	}
}

void delete_msg_from_queue(msg_t *head, msg_t *target){
	msg_t *temp, *temp2;

	if(is_msg_list_empty(head)){
		//printf("Error:This is an empty list!\n");
	}else{
		temp = head->prev;
		while(temp != head){
			if(temp == target){
				temp2 = temp->prev;
	
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				target->next = NULL;
				target->prev = NULL;

				temp = temp2;
				continue;
			}
			temp = temp->prev;
		}
		//printf("Error:There is no such msg!\n");
	}
}

void msg_queue_init(void){
	MsgQueue = (msg_t *)os_mem_alloc((UINT8)sizeof(msg_t));
	if(MsgQueue){
		//printf("Message queue created!\n");
	}else{
		//printf("Error:creating message queue failed!\n");
		return;
	}

	MsgList = (msg_t *)os_mem_alloc((UINT8)sizeof(msg_t));
	if(MsgQueue){
		//printf("Idle message list created!\n");
	}else{
		//printf("Error:creating idle message list failed!\n");
		return;
	}

	MsgQueue->next = MsgQueue;
	MsgQueue->prev = MsgQueue;
	MsgList->next = MsgList;
	MsgList->prev = MsgList;
}

msg_t *get_msg(void){
	msg_t *tar;

	if(is_msg_list_empty(MsgList)){
		return NULL;
	}else{
		tar = MsgList->prev;
		delete_msg_from_queue(MsgList, tar);
		return tar;
	}
}

UINT8 msg_create(msg_t **NewMsg){
	if(is_msg_list_empty(MsgList)){
		*NewMsg = (msg_t *)os_mem_alloc((UINT8)sizeof(msg_t));
		if(*NewMsg){
			return 1;
		}else{
			return 0;
		}
	}else{
		//printf("get msg from idle list!\n");
		*NewMsg = get_msg();
		if(*NewMsg){
			return 1;
		}else{
			return 0;
		}
	}
}

UINT8 msg_send(msg_t *msg, UINT8 *data, UINT8 destinationPID){

	msg->data = data;
	msg->pid = destinationPID;

	enter_critical_section();
	insert_msg_to_queue(MsgQueue, msg);
	exit_critical_section();

	return 1;
}

void msg_pending(UINT32 timeout, UINT8 pid){
	msg_t *temp, *temp2;
	process_control_block_t *target;

	target = find_pcb(pid);
	target->timeout = timeout;

	process_wait(target);
	//os_schedule();
	
	/** 
	 ** There is no message, when the process that invoked
	 ** msg_pending is rescheduled. It means that process
	 ** was rescheduled by OS for the reason of timeout.
	 **/
	if(is_msg_list_empty(target->queue) && !target->timeout){
		//printf("Return timeout!\n"); 
		target->flag = SET_PROC_WAKEUP_FROM_MSG(target->flag);
	}else{
		target->flag = SET_PROC_WAKEUP_FROM_MSG(target->flag);
		temp = target->queue->next;	//Get messages in a loop
		while(temp != target->queue){
			//printf("%s\n", temp->data);
			temp2 = temp->next;
			msg_recycle(target->queue, temp);
			temp = temp2;
		}
	}
}

msg_t *msg_accept(void){	//When there is no message in queue, function return immediately

	return 0;
}

void msg_handler(void){
	msg_t *tar;
	process_control_block_t *temp;

	if(!is_msg_list_empty(MsgQueue)){
		tar = MsgQueue->prev;
		temp = find_pcb(tar->pid);

		enter_critical_section();
		delete_msg_from_queue(MsgQueue, tar);
		exit_critical_section();

		insert_msg_to_queue(temp->queue, tar);
		temp->flag = SET_PROC_WAKEUP_FROM_MSG(temp->flag);
	}else{
		return;
	}
}

void msg_recycle(msg_t *head, msg_t *idle){

	delete_msg_from_queue(head, idle);
	
	enter_critical_section();
	insert_msg_to_queue(MsgList, idle);
	exit_critical_section();

	idle->pid = 0;
	//os_mem_free(idle->data);
}

void process_msg_queue_init(process_control_block_t *proc){
	proc->queue = (msg_t *)os_mem_alloc((UINT8)sizeof(msg_t));

	proc->queue->next = proc->queue;
	proc->queue->prev = proc->queue;
}

void msg_list_free(void){
	msg_t *temp;

	if(!is_msg_list_empty(MsgQueue)){
		temp = MsgQueue->prev;
		while(temp != MsgQueue){
			temp->prev->next = MsgQueue;
			MsgQueue->prev = temp->prev;
			os_mem_free(temp);
			temp = temp->prev;
		}
		os_mem_free(MsgQueue);
	}else{
		os_mem_free(MsgQueue);
	}

	if(!is_msg_list_empty(MsgList)){
		temp = MsgList->prev;
		while(temp != MsgList){
			temp->prev->next = MsgList;
			MsgList->prev = temp->prev;
			os_mem_free(temp);
			temp = temp->prev;
		}
		os_mem_free(MsgList);
	}else{
		os_mem_free(MsgList);
	}
}
