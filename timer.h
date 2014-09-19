#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include "portable.h"

typedef struct os_timer{
	UINT8			pid;
	UINT32 			delay;
	struct os_timer	*next;
	struct os_timer	*prev;
}os_timer_t;

extern struct os_timer *timerList;

void timer_list_init(void);

#endif
