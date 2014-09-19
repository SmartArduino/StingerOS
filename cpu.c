#include "cpu.h"

/*******************************************
* Local variables 局部变量
********************************************/
/*
#if OS_TMR_EN > 0
    static INT16U OSTmrCtr;
#endif			
*/
/*******************************************
*
********************************************/
UINT32 *task_stack_init(void (*task)(), void  *parg, UINT32  *ptos){
    UINT32 *stk;
                                                                            
    stk = ptos;                                                         /*  Load stack pointer          */
                                                                        /*  装载堆栈指针                */                                    

                                                                        /*  Registers stacked as if     */
                                                                        /*  auto-saved on exception     */             
	                                                                	/*  模拟成异常，自动把寄存器压栈*/

    *(stk) = (UINT32)0x01000000L;                                       /*  xPSR                        */ 
    *(--stk) = (UINT32)task;                                            /*  Entry Point of the task     */
                                                                        /*  任务入口地址                */
    *(--stk) = (UINT32)0xFFFFFFFEL;                                     /*  R14 (LR)  (init value will  */
                                                                        /*  cause fault if ever used)   */
                                                                           
    *(--stk) = (UINT32)0x12121212L;                                     /*  R12                         */
    *(--stk) = (UINT32)0x03030303L;                                     /*  R3                          */
    *(--stk) = (UINT32)0x02020202L;                                     /*  R2                          */
    *(--stk) = (UINT32)0x01010101L;                                     /*  R1                          */
    *(--stk) = (UINT32)parg;                                            /*  R0 : argument  输入参数     */
                                                                        /*  Remaining registers saved on*/
                                                                        /*  process stack               */   
	                                                                	/*  剩下的寄存器保存到堆栈      */

    *(--stk) = (UINT32)0x11111111L;                                     /*  R11                         */
    *(--stk) = (UINT32)0x10101010L;                                     /*  R10                         */
    *(--stk) = (UINT32)0x09090909L;                                     /*  R9                          */
    *(--stk) = (UINT32)0x08080808L;                                     /*  R8                          */
    *(--stk) = (UINT32)0x07070707L;                                     /*  R7                          */
    *(--stk) = (UINT32)0x06060606L;                                     /*  R6                          */
    *(--stk) = (UINT32)0x05050505L;                                     /*  R5                          */
    *(--stk) = (UINT32)0x04040404L;                                     /*  R4                          */

    return(stk);
}

