;*********************************************************************************************************
;  Declarations for the interrupt handlers that are used by the application.              
;  ��Ӧ�����õ�������                                       
;*********************************************************************************************************
    EXTERN  current
	EXTERN	osEnterSum
	EXTERN  next

;*********************************************************************************************************
;  Declarations for the exported functions             
;  ����ⲿ����                          
;*********************************************************************************************************
    EXPORT  OSStartHighRdy
    EXPORT  OSCtxSw
    EXPORT  OSIntCtxSw
    EXPORT  OSPendSV
    EXPORT  OS_ENTER_CRITICAL
    EXPORT  OS_EXIT_CRITICAL                  
    EXPORT  intDisAll      
           
;*********************************************************************************************************
;  Registers or macros used by this file                                
;  ���ļ��õ��ļĴ����ͺ�                             
;*********************************************************************************************************							    
NVIC_INT_CTRL           EQU     0xE000ED04                              ;  Interrupt control state  
                                                                        ;  register.
                                                                        ;  �жϿ��ƼĴ���

NVIC_SYSPRI2            EQU     0xE000ED20                              ;  System priority register (2)
                                                                        ;  ϵͳ���ȼ��Ĵ�����2��

NVIC_PENDSV_PRI         EQU     0xFFFF0000                              ;  PendSV and system tick priority
                                                                        ;  (Both are lowest,0xff)  
                                                                        ;  ����жϺ�ϵͳ�����ж�
                                                                        ;  ����Ϊ��ͣ�0xff��.
NVIC_PENDSVSET          EQU     0x10000000                              ;  Value to trigger PendSV  
                                                                        ;  exception.��������жϵ�ֵ.

OS_CRITICAL_INT_PRIO    EQU     (1 << 5)		                		;  Critical setions manage priority 
                                                                        ;  (equal or bigger number managed) 
                                                                        ;  �ٽ�����������ȼ�����ֵ����
        PRESERVE8                                                       ;  ����ڱ�����
	         
		AREA    PORT, CODE, READONLY
        THUMB	
    
;*********************************************************************************************************
;*********************************************************************************************************
OS_ENTER_CRITICAL
	IF OS_CRITICAL_INT_PRIO > 0
		PUSH 	{R1,R2} 	                
		LDR 	R1, =OS_CRITICAL_INT_PRIO                               ;  Disable priorities between 
                                                                        ;  OS_CRITICAL_INT_PRIO & 0xFF 
                                                                             			 				
        MSR 	BASEPRI,  R1                                            ;  �������ȼ���OS_CRITICAL_INT 
                                                                        ;  _PRIO��0xFF���ж�      			 				 
    ELSE
	CPSID   I                                                       ;  Disable all the interrupts
                                                                        ;  ���������ж�
        PUSH 	{R1,R2}      
    ENDIF
    
    LDR     R1, __OS_EnterSum	                                        ;  OsEnterSum++
    LDRB    R2, [R1]
    ADD     R2, R2, #1
    STRB    R2, [R1]
    POP     {R1,R2}
    BX      LR

;*********************************************************************************************************
;
;*********************************************************************************************************
OS_EXIT_CRITICAL
	PUSH    {R1, R2}
	LDR     R1, __OS_EnterSum                                     		;  OsEnterSum--
    LDRB    R2, [R1]
    SUB     R2, R2, #1
    STRB    R2, [R1]
	MOV     R1,  #0    
	CMP     R2,  #0			                                			;  if OsEnterSum=0,enable 
                                                                        ;  interrupts.  
     	                                                                ;  ���OsEnterSum=0�����жϡ�
        
    IF OS_CRITICAL_INT_PRIO > 0 
 	    IT EQ 
  	    MSREQ  BASEPRI, R1 
	ELSE
        IT EQ 
        MSREQ  PRIMASK, R1     
	ENDIF
        
        POP     {R1, R2}
        BX      LR
        

;*********************************************************************************************************
;
;*********************************************************************************************************
OSStartHighRdy

    LDR     R4, =NVIC_SYSPRI2                                           ;  set the PendSV exception 
                                                                        ;  priority����PendSV���ȼ�
    LDR     R5, =NVIC_PENDSV_PRI	
    STR     R5, [R4]
	    
;    MOV     R4, #0                                                      ;  set the PSP to 0 for initial  
                                                                        ;  context switch call ʹPSP����0
	LDR     R4, __OS_TCBCur
	LDR		R4, [R4]
	ADD		R4, R4, #0x20
    MSR     PSP, R4

;    LDR     R4, =NVIC_INT_CTRL                                          ;  trigger the PendSV exception
                                                                        ;  ��������ж�
;    LDR     R5, =NVIC_PENDSVSET
;    STR     R5, [R4]

    CPSIE   I                                                           ;  enable interrupts at processor 
                                                                        ;  levelʹ���������ȼ����ж�
;OSStartHang
;    B       OSStartHang     
	BX		LR
	NOP

;*********************************************************************************************************
;*********************************************************************************************************
OSCtxSw
    PUSH    {R4, R5}
    LDR     R4, =NVIC_INT_CTRL                                          ;  trigger the PendSV exception
                                                                        ;  ��������ж�
    LDR     R5, =NVIC_PENDSVSET
    STR     R5, [R4]
    POP     {R4, R5}
    BX      LR


;*********************************************************************************************************
;*********************************************************************************************************
OSIntCtxSw
    PUSH    {R4, R5}
    LDR     R4, =NVIC_INT_CTRL                                          ;  trigger the PendSV exception
                                                                        ;  ��������ж�
    LDR     R5, =NVIC_PENDSVSET
    STR     R5, [R4]
    POP     {R4, R5}
    BX      LR
    NOP


;*********************************************************************************************************
;*********************************************************************************************************
OSPendSV
    IF OS_CRITICAL_INT_PRIO > 0	                                        ;  disable interupt �����ж�
        MRS R3, BASEPRI
        LDR R1, =OS_CRITICAL_INT_PRIO
	MSR BASEPRI, R1
    ELSE
       	MRS     R3, PRIMASK                
       	CPSID   I
    ENDIF

    MRS     R0, PSP                                                     ;  PSP is process stack pointer  
                                                                        ;  PSP������Ķ�ջָ��
;    CBZ     R0, OSPendSV_nosave                                         ;  skip register save the first 
                                                                        ;  time��һ����������

    SUB     R0, R0, #0x20                                               ;  save remaining regs r4-11 on 
                                                                        ;  process stack ����r4-r11
    STM     R0, {R4-R11}

    LDR     R4, __OS_TCBCur                                             ;  OSTCBCur->OSTCBStkPtr = SP;
    LDR     R4, [R4]
    STR     R0, [R4]                                                    ;  R0 is SP of process being 
                                                                        ;  switched outR0�Ǳ��л���������
                                                                        ;  �Ķ�ջָ��
;OSPendSV_nosave
	;NOP 	  
    LDR     R4, __OS_TCBCur                                             ;  OSTCBCur  = OSTCBNext;
    LDR     R6, __OS_TCBNext      
    LDR     R6, [R6]
    STR     R6, [R4]

    LDR     R0, [R6]                                                    ;  SP = OSTCBHighRdy->OSTCBStkPtr;
    LDM     R0, {R4-R11}                                                ;  restore r4-11 from new process
                                                                        ;  stack ��������Ķ�ջ�ָ�r4-r11
    ADD     R0, R0, #0x20
    MSR     PSP, R0                                                     ;  load PSP with new process SP
                                                                        ;  ��������Ķ�ջ�ָ�PSP
 	   	
    ORR     LR, LR, #0x04                                               ;  ensure exception return uses 
                                                                        ;  PSPȷ�����غ�ʹ��PSP
        
    IF OS_CRITICAL_INT_PRIO > 0                                         ;  restore interrupts �ָ��ж�  
        MSR 	BASEPRI,  R3
    ELSE
	MSR     PRIMASK, R3
    ENDIF
		   
    BX      LR                                                          ; exception return will restore 
                                                                        ;  remaining context 
                                                                        ;  ����ʱ��ָ�ʣ�µ�������
    NOP


;*********************************************************************************************************
;*********************************************************************************************************
intDisAll
    CPSID   I
    BX      LR
    
    
;*********************************************************************************************************
;  POINTERS TO VARIABLES
;  ����ָ��  
;*********************************************************************************************************
    ;DATA
	NOP																	;   ��NOP��Ϊ���ڴ���룬�������A1616E�Ĵ���
__OS_TCBCur
    DCD		current

__OS_EnterSum
	DCD		osEnterSum

__OS_TCBNext
	DCD		next
    END

