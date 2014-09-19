#ifndef _STINGER_H_
#define _STINGER_H_

#include <stdio.h>
//#include <unistd.h>
#include "portable.h"

#define stinger_start	main

#define WAITING			0
#define RUNNING			1

#define NEED_SCHEDULE	0x01
#define WAIT_FOR_MSG	0x02
#define DEFAULT_SLICE	100

#define IDLE_FIRST_RUN	0x01
#define IS_IDLE_FIRST_RUN(tag)			(tag & IDLE_FIRST_RUN)
#define SET_IDLE_RUNNING(tag)			(tag & 0xFE)

#define PROC_STACK_SIZE	64

#define SET_PROC_WAITING(flag)			(flag & 0xFE)
#define SET_PROC_RUNNING(flag)			(flag | NEED_SCHEDULE)
#define GET_PROC_STATUS(flag)			(flag & 0x01)
#define SET_PROC_WAIT_MSG(flag)			(flag | WAIT_FOR_MSG)
#define SET_PROC_WAKEUP_FROM_MSG(flag)	(flag & 0xFD)
#define IS_PROC_WAIT_FOR_MSG(flag)		(flag & WAIT_FOR_MSG)

typedef struct pcb{
	UINT32			*ptos;
	UINT8			pid;
	UINT8			prio;
	UINT32			slice;
	UINT8			status;
	UINT8			flag;
	UINT32			timeout;
	struct pcb		*next;
	struct pcb		*prev;
	struct msg		*queue;
	void			(*proc)(void);
}process_control_block_t;

extern struct pcb RunningProcList, WaitingProcList;
extern struct pcb *current, *idle, *next;

UINT8 is_list_empty(process_control_block_t *head);
UINT8 is_there_only_init_proc(void);
void free_list_mem(process_control_block_t *head);
void process_idle(void);
UINT8 process_create(UINT8 pid, 
					 UINT8 prio, 
					 process_control_block_t *pNew, 
					 void (*proc)(void), 
					 UINT32 *pStack);
process_control_block_t *find_pcb(UINT8 pid);
void process_init(UINT32 *stackForIdle);
void process_wait(process_control_block_t *target);
void os_schedule(void);
int need_schedule(void);
UINT32 *get_stack_end(UINT32 *pStk, UINT32 size);

#endif
