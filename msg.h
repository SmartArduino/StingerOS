#ifndef _MSG_H_
#define _MSG_H_

#include  <stdio.h>
#include "stinger.h"

typedef struct msg{
	UINT8		pid;
	UINT8		*data;
	struct msg	*next;
	struct msg	*prev;
}msg_t;

extern struct msg *MsgQueue, *MsgList;

void msg_queue_init(void);
UINT8 is_msg_list_empty(msg_t *head);
void insert_msg_to_queue(msg_t *head, msg_t *new);
void delete_msg_from_queue(msg_t *head, msg_t *target);
UINT8 msg_create(msg_t **NewMsg);
msg_t *get_msg(void);
UINT8 msg_send(msg_t *msg, UINT8 *data, UINT8 destinationPID);
void msg_pending(UINT32 timeout, UINT8 pid);
void msg_handler(void);
void msg_recycle(msg_t *head, msg_t *idle);
void msg_list_free(void);
void process_msg_queue_init(process_control_block_t *proc);

#endif
