#include "hal.h"
//#include "stinger.h"
#include "cpu.h"
#include "pool.h"
#include "includes.h"

void *os_mem_alloc(UINT8 bytes){
	void *ptr = NULL;

	ptr = pool_mem_alloc(bytes);

	return ptr;
}

void os_mem_free(void *ptr){
	pool_mem_free(ptr);
}

void clock_tick(void){
	process_control_block_t *temp;
	
	enter_critical_section();

	if(!is_list_empty(&WaitingProcList)){
		temp = WaitingProcList.next;
		while(temp != &WaitingProcList){
			//Firstly, we should check whether the process is waiting for message.
			if(IS_PROC_WAIT_FOR_MSG(temp->flag)){	
				if(--(temp->timeout) == 0){
					temp->flag = SET_PROC_RUNNING(temp->flag);
					temp->flag = SET_PROC_WAKEUP_FROM_MSG(temp->flag);
				}
			}
			temp = temp->next;
		}
	}

	if(current->slice){
		(current->slice)--;
		if(current->slice == 0){
			current->flag = SET_PROC_WAITING(current->flag);
			if(current->pid == 0){
				//current->flag = SET_PROC_RUNNING(current->flag);
				//current->slice = DEFAULT_SLICE;
			}
			//printf("slice of current process has count down to zero! %d\n", current->pid);
		}
	}

	exit_critical_section();

	if(need_schedule()){
		OSIntCtxSw();
	}
}

void enter_critical_section(void){
	OS_ENTER_CRITICAL();
}

void exit_critical_section(void){
	OS_EXIT_CRITICAL();
}

void switch_context(process_control_block_t *pNextProc){
	next=pNextProc;
}

void tick_init(void){
    SysTickPeriodSet((UINT32)(SysCtlClockGet() / OS_TICKS_PER_SEC) -1 );
    SysTickEnable();
    SysTickIntEnable();
}
