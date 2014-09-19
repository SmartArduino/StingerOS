#ifndef _HAL_H_
#define _HAL_H_

#include <stdlib.h>
#include "stinger.h"
#include "portable.h"

#define OS_TICKS_PER_SEC 20

void	*os_mem_alloc(UINT8 bytes);
void	os_mem_free(void *ptr);
void	clock_tick(void);
void	enter_critical_section(void);
void	exit_critical_section(void);
void	switch_context(process_control_block_t *pNextProc);
void	tick_init(void);  

#endif

