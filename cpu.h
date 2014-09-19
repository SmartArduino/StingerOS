#ifndef _CPU_H_
#define _CPU_H_

#include "portable.h"

extern unsigned long osEnterSum;

void OS_ENTER_CRITICAL(void);
void OS_EXIT_CRITICAL(void);
void OSCtxSw(void);
void OSIntCtxSw(void);
void OSPendSV(void); 
UINT32 *task_stack_init(void (*task)(), void  *parg, UINT32  *ptos);

#endif

