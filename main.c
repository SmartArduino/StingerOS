/***************************************
* cc2420测试代码
* Nicholas Wang
* 2011.11.15
***************************************/
#include <stdio.h>
#include "includes.h"
#include "global.h"
#include "cc2420.h"
#include "common.h"
#include "pool.h"
#include "random.h"
#include "stack.h"
#include "macro.h"

#include "stinger.h"
#include "msg.h"
#include "user.h"
#include "timer.h"
#include "hal.h"

/***************************************
* 外部函数
***************************************/
void (void);
/***************************************
* 全局变量
***************************************/
#if	END_POINT_TX > 0 
	//unsigned char dataTX[TXLEN];
	// long型的变量，用作标志判断
	// 总共可做32个标志
	// 第0位：保留
	// 第1位：发送数据测试位，1为发送数据，0为不发送
	unsigned long flag=0;
	//unsigned char dataRAM[TXLEN];
#endif

	unsigned char serialNum=0;

#if	END_POINT_RX > 0
	unsigned char panID[2]={0x23,0x45};
	unsigned char shortAddr[2]={0x15,0x73};
#endif

/***************************************
* 函数功能：lm3s3748管脚初始化
* 输入参数：无
* 返 回 值：无
***************************************/
void board_init(void){
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	// GPIO管脚使能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA | 
						   SYSCTL_PERIPH_GPIOB | 
						   SYSCTL_PERIPH_GPIOC | 
						   SYSCTL_PERIPH_GPIOD | 
						   SYSCTL_PERIPH_GPIOE |
						   SYSCTL_PERIPH_GPIOF |
						   SYSCTL_PERIPH_GPIOG);
	// 串口使能
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	// 处理器中断使能
    IntMasterEnable();
	// UART数据收发管脚设置.
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // 串口通信配置：
	// 波特率：115,200，数据位：8，校验位：N，停止位：1
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
    // 串口中断使能
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	// 串口控制台初始化
	UARTStdioInit(0);

	//初始化第一片cc2420	 
	GPIOPinTypeGPIOOutput(CHIP_1_PORT, CHIP_1_SCLK);	//GPA2-SCLK
	GPIOPinTypeGPIOOutput(CHIP_1_PORT, CHIP_1_CS);	//GPA3-CS
	GPIOPinTypeGPIOOutput(CHIP_1_PORT, CHIP_1_SI);	//GPA5-SI
	GPIOPinTypeGPIOInput(CHIP_1_PORT, CHIP_1_SO);	//GPA4-SO

	//初始化第二片cc2420
	GPIOPinTypeGPIOOutput(CHIP_2_PORT, CHIP_2_CS);	//GPC5-CS
	GPIOPinTypeGPIOOutput(CHIP_2_PORT, CHIP_2_SCLK);//GPC4-SCLK
	GPIOPinTypeGPIOOutput(CHIP_2_PORT, CHIP_2_SI);	//GPC7-SI
	GPIOPinTypeGPIOInput(CHIP_2_PORT, CHIP_2_SO);	//GPC6-SO

	//初始化第三片cc2420
	GPIOPinTypeGPIOOutput(CHIP_3_PORT, CHIP_3_CS);	//GPE1-CS
	GPIOPinTypeGPIOOutput(CHIP_3_PORT, CHIP_3_SCLK);	//GPE0-SCLK
	GPIOPinTypeGPIOOutput(CHIP_3_PORT, CHIP_3_SI);	//GPE3-SI
	GPIOPinTypeGPIOInput(CHIP_3_PORT, CHIP_3_SO);	//GPE6-SO

	//初始化第四片cc2420
	GPIOPinTypeGPIOOutput(CHIP_4_PORT, CHIP_4_CS);	//GPB1-CS
	GPIOPinTypeGPIOOutput(CHIP_4_PORT, CHIP_4_SCLK);//GPB0-SCLK
	GPIOPinTypeGPIOOutput(CHIP_4_PORT, CHIP_4_SI);	//GPB3-SI
	GPIOPinTypeGPIOInput(CHIP_4_PORT, CHIP_4_SO);	//GPB2-SO

	//初始化第五片cc2420
	GPIOPinTypeGPIOOutput(CHIP_5_PORT, CHIP_5_CS);	//GPD1-CS
	GPIOPinTypeGPIOOutput(CHIP_5_PORT, CHIP_5_SCLK);//GPD0-SCLK
	GPIOPinTypeGPIOOutput(CHIP_5_PORT, CHIP_5_SI);	//GPD3-SI
	GPIOPinTypeGPIOInput(CHIP_5_PORT, CHIP_5_SO);	//GPD2-SO

	//每个cc2420的cs初始化为高电平
	GPIOPinWrite(CHIP_1_PORT, CHIP_1_CS, CHIP_1_CS);
	GPIOPinWrite(CHIP_2_PORT, CHIP_2_CS, CHIP_2_CS);
	GPIOPinWrite(CHIP_3_PORT, CHIP_3_CS, CHIP_3_CS);
	GPIOPinWrite(CHIP_4_PORT, CHIP_4_CS, CHIP_4_CS);
	GPIOPinWrite(CHIP_5_PORT, CHIP_5_CS, CHIP_5_CS);

	//cc2420的VREG_EN设置
	GPIOPinTypeGPIOOutput(CHIP_1_VREGEN_PORT, CHIP_1_VREGEN_PIN);	//第一片cc2420的VREG_EN:GPG6
	GPIOPinTypeGPIOOutput(CHIP_2_VREGEN_PORT, CHIP_2_VREGEN_PIN);	//第二片cc2420的VREG_EN:GPG2
	GPIOPinTypeGPIOOutput(CHIP_3_VREGEN_PORT, CHIP_3_VREGEN_PIN);	//第三片cc2420的VREG_EN:GPE4
	GPIOPinTypeGPIOOutput(CHIP_4_VREGEN_PORT, CHIP_4_VREGEN_PIN);	//第四片cc2420的VREG_EN:GPB6
	GPIOPinTypeGPIOOutput(CHIP_5_VREGEN_PORT, CHIP_5_VREGEN_PIN);	//第五片cc2420的VREG_EN:GPD7

	
	#if END_POINT_RX > 0
	//cc2420的FIFOP管脚使能
	GPIOPinTypeGPIOInput(CHIP_1_FIFOP_PORT, CHIP_1_FIFOP_PIN);	//第一片cc2420的FIFOP脚
	GPIOPinTypeGPIOInput(CHIP_2_FIFOP_PORT, CHIP_2_FIFOP_PIN);	//第二片cc2420的FIFOP脚
	GPIOPinTypeGPIOInput(CHIP_3_FIFOP_PORT, CHIP_3_FIFOP_PIN);	//第三片cc2420的FIFOP脚
	GPIOPinTypeGPIOInput(CHIP_4_FIFOP_PORT, CHIP_4_FIFOP_PIN);	//第四片cc2420的FIFOP脚
	GPIOPinTypeGPIOInput(CHIP_5_FIFOP_PORT, CHIP_5_FIFOP_PIN);	//第五片cc2420的FIFOP脚

	GPIOPinTypeGPIOInput(CHIP_1_FIFO_PORT, CHIP_1_FIFO_PIN);	//第一片cc2420的FIFO脚
	GPIOPinTypeGPIOInput(CHIP_2_FIFO_PORT, CHIP_2_FIFO_PIN);	//第二片cc2420的FIFO脚
	GPIOPinTypeGPIOInput(CHIP_3_FIFO_PORT, CHIP_3_FIFO_PIN);	//第三片cc2420的FIFO脚
	GPIOPinTypeGPIOInput(CHIP_4_FIFO_PORT, CHIP_4_FIFO_PIN);	//第四片cc2420的FIFO脚
	GPIOPinTypeGPIOInput(CHIP_5_FIFO_PORT, CHIP_5_FIFO_PIN);	//第五片cc2420的FIFO脚
	#endif

	//CCA
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);	//第一片cc2420的CCA
	//GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);	//第二片cc2420的CCA

	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);	//第一片cc2420的SFD脚

}

/***************************************
* 函数功能：串口数据发送
* 输入参数：pucBuffer
*			ulCount
* 返 回 值：无
***************************************/
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount){

    while(ulCount--){
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}

/***************************************
* 函数功能：串口中断ISR
* 输入参数：无
* 返 回 值：无
***************************************/
void UARTIntHandler(void){
	unsigned long ulStatus;
	#if END_POINT_TX>0	//配置为数据发送端
		unsigned char ucData;
	#endif

    // 获取中断状态
    ulStatus = UARTIntStatus(UART0_BASE, true);

    // Clear the asserted interrupts.
    UARTIntClear(UART0_BASE, ulStatus);

	// 循环等待输入缓冲区中的数据
    while(UARTCharsAvail(UART0_BASE)){
		// 读取缓冲区中的一个字节
		#if END_POINT_TX>0	//配置为数据发送端
			ucData = UARTCharGetNonBlocking(UART0_BASE);
			if(ucData == 0x01){
				flag |= SEND_DATA;
			}
		#endif

		UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
	}
	UARTprintf("\nReceive from PC.\n");
}

/***************************************
* 主函数，发送数据
***************************************/
int main(void){
	
	board_init();
	pool_create(10240);

	process_init((unsigned long *)os_mem_alloc(PROC_STACK_SIZE));
	msg_queue_init();
	timer_list_init();
	user_process_init();
	();
	tick_init();

	for(;;){
		os_schedule();
		delay(1000);
	}

	//pool_destroy();
}


