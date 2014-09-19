#include "includes.h"
#include "timer.h"
#include "hal.h"

struct os_timer *timerList = NULL;

void timer_list_init(void){
	timerList = (os_timer_t *)os_mem_alloc((UINT8)sizeof(os_timer_t));
	if(!timerList){
		UARTprintf("Error:create timer list failed!\n");
	}

	timerList->next = timerList;
	timerList->prev = timerList;
}
