/***************************************
* cc2420���Դ���
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
* �ⲿ����
***************************************/
void (void);
/***************************************
* ȫ�ֱ���
***************************************/
#if	END_POINT_TX > 0 
	//unsigned char dataTX[TXLEN];
	// long�͵ı�����������־�ж�
	// �ܹ�����32����־
	// ��0λ������
	// ��1λ���������ݲ���λ��1Ϊ�������ݣ�0Ϊ������
	unsigned long flag=0;
	//unsigned char dataRAM[TXLEN];
#endif

	unsigned char serialNum=0;

#if	END_POINT_RX > 0
	unsigned char panID[2]={0x23,0x45};
	unsigned char shortAddr[2]={0x15,0x73};
#endif

/***************************************
* �������ܣ�lm3s3748�ܽų�ʼ��
* �����������
* �� �� ֵ����
***************************************/
void board_init(void){
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	// GPIO�ܽ�ʹ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA | 
						   SYSCTL_PERIPH_GPIOB | 
						   SYSCTL_PERIPH_GPIOC | 
						   SYSCTL_PERIPH_GPIOD | 
						   SYSCTL_PERIPH_GPIOE |
						   SYSCTL_PERIPH_GPIOF |
						   SYSCTL_PERIPH_GPIOG);
	// ����ʹ��
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	// �������ж�ʹ��
    IntMasterEnable();
	// UART�����շ��ܽ�����.
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // ����ͨ�����ã�
	// �����ʣ�115,200������λ��8��У��λ��N��ֹͣλ��1
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
    // �����ж�ʹ��
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	// ���ڿ���̨��ʼ��
	UARTStdioInit(0);

	//��ʼ����һƬcc2420	 
	GPIOPinTypeGPIOOutput(CHIP_1_PORT, CHIP_1_SCLK);	//GPA2-SCLK
	GPIOPinTypeGPIOOutput(CHIP_1_PORT, CHIP_1_CS);	//GPA3-CS
	GPIOPinTypeGPIOOutput(CHIP_1_PORT, CHIP_1_SI);	//GPA5-SI
	GPIOPinTypeGPIOInput(CHIP_1_PORT, CHIP_1_SO);	//GPA4-SO

	//��ʼ���ڶ�Ƭcc2420
	GPIOPinTypeGPIOOutput(CHIP_2_PORT, CHIP_2_CS);	//GPC5-CS
	GPIOPinTypeGPIOOutput(CHIP_2_PORT, CHIP_2_SCLK);//GPC4-SCLK
	GPIOPinTypeGPIOOutput(CHIP_2_PORT, CHIP_2_SI);	//GPC7-SI
	GPIOPinTypeGPIOInput(CHIP_2_PORT, CHIP_2_SO);	//GPC6-SO

	//��ʼ������Ƭcc2420
	GPIOPinTypeGPIOOutput(CHIP_3_PORT, CHIP_3_CS);	//GPE1-CS
	GPIOPinTypeGPIOOutput(CHIP_3_PORT, CHIP_3_SCLK);	//GPE0-SCLK
	GPIOPinTypeGPIOOutput(CHIP_3_PORT, CHIP_3_SI);	//GPE3-SI
	GPIOPinTypeGPIOInput(CHIP_3_PORT, CHIP_3_SO);	//GPE6-SO

	//��ʼ������Ƭcc2420
	GPIOPinTypeGPIOOutput(CHIP_4_PORT, CHIP_4_CS);	//GPB1-CS
	GPIOPinTypeGPIOOutput(CHIP_4_PORT, CHIP_4_SCLK);//GPB0-SCLK
	GPIOPinTypeGPIOOutput(CHIP_4_PORT, CHIP_4_SI);	//GPB3-SI
	GPIOPinTypeGPIOInput(CHIP_4_PORT, CHIP_4_SO);	//GPB2-SO

	//��ʼ������Ƭcc2420
	GPIOPinTypeGPIOOutput(CHIP_5_PORT, CHIP_5_CS);	//GPD1-CS
	GPIOPinTypeGPIOOutput(CHIP_5_PORT, CHIP_5_SCLK);//GPD0-SCLK
	GPIOPinTypeGPIOOutput(CHIP_5_PORT, CHIP_5_SI);	//GPD3-SI
	GPIOPinTypeGPIOInput(CHIP_5_PORT, CHIP_5_SO);	//GPD2-SO

	//ÿ��cc2420��cs��ʼ��Ϊ�ߵ�ƽ
	GPIOPinWrite(CHIP_1_PORT, CHIP_1_CS, CHIP_1_CS);
	GPIOPinWrite(CHIP_2_PORT, CHIP_2_CS, CHIP_2_CS);
	GPIOPinWrite(CHIP_3_PORT, CHIP_3_CS, CHIP_3_CS);
	GPIOPinWrite(CHIP_4_PORT, CHIP_4_CS, CHIP_4_CS);
	GPIOPinWrite(CHIP_5_PORT, CHIP_5_CS, CHIP_5_CS);

	//cc2420��VREG_EN����
	GPIOPinTypeGPIOOutput(CHIP_1_VREGEN_PORT, CHIP_1_VREGEN_PIN);	//��һƬcc2420��VREG_EN:GPG6
	GPIOPinTypeGPIOOutput(CHIP_2_VREGEN_PORT, CHIP_2_VREGEN_PIN);	//�ڶ�Ƭcc2420��VREG_EN:GPG2
	GPIOPinTypeGPIOOutput(CHIP_3_VREGEN_PORT, CHIP_3_VREGEN_PIN);	//����Ƭcc2420��VREG_EN:GPE4
	GPIOPinTypeGPIOOutput(CHIP_4_VREGEN_PORT, CHIP_4_VREGEN_PIN);	//����Ƭcc2420��VREG_EN:GPB6
	GPIOPinTypeGPIOOutput(CHIP_5_VREGEN_PORT, CHIP_5_VREGEN_PIN);	//����Ƭcc2420��VREG_EN:GPD7

	
	#if END_POINT_RX > 0
	//cc2420��FIFOP�ܽ�ʹ��
	GPIOPinTypeGPIOInput(CHIP_1_FIFOP_PORT, CHIP_1_FIFOP_PIN);	//��һƬcc2420��FIFOP��
	GPIOPinTypeGPIOInput(CHIP_2_FIFOP_PORT, CHIP_2_FIFOP_PIN);	//�ڶ�Ƭcc2420��FIFOP��
	GPIOPinTypeGPIOInput(CHIP_3_FIFOP_PORT, CHIP_3_FIFOP_PIN);	//����Ƭcc2420��FIFOP��
	GPIOPinTypeGPIOInput(CHIP_4_FIFOP_PORT, CHIP_4_FIFOP_PIN);	//����Ƭcc2420��FIFOP��
	GPIOPinTypeGPIOInput(CHIP_5_FIFOP_PORT, CHIP_5_FIFOP_PIN);	//����Ƭcc2420��FIFOP��

	GPIOPinTypeGPIOInput(CHIP_1_FIFO_PORT, CHIP_1_FIFO_PIN);	//��һƬcc2420��FIFO��
	GPIOPinTypeGPIOInput(CHIP_2_FIFO_PORT, CHIP_2_FIFO_PIN);	//�ڶ�Ƭcc2420��FIFO��
	GPIOPinTypeGPIOInput(CHIP_3_FIFO_PORT, CHIP_3_FIFO_PIN);	//����Ƭcc2420��FIFO��
	GPIOPinTypeGPIOInput(CHIP_4_FIFO_PORT, CHIP_4_FIFO_PIN);	//����Ƭcc2420��FIFO��
	GPIOPinTypeGPIOInput(CHIP_5_FIFO_PORT, CHIP_5_FIFO_PIN);	//����Ƭcc2420��FIFO��
	#endif

	//CCA
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);	//��һƬcc2420��CCA
	//GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);	//�ڶ�Ƭcc2420��CCA

	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);	//��һƬcc2420��SFD��

}

/***************************************
* �������ܣ��������ݷ���
* ���������pucBuffer
*			ulCount
* �� �� ֵ����
***************************************/
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount){

    while(ulCount--){
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}

/***************************************
* �������ܣ������ж�ISR
* �����������
* �� �� ֵ����
***************************************/
void UARTIntHandler(void){
	unsigned long ulStatus;
	#if END_POINT_TX>0	//����Ϊ���ݷ��Ͷ�
		unsigned char ucData;
	#endif

    // ��ȡ�ж�״̬
    ulStatus = UARTIntStatus(UART0_BASE, true);

    // Clear the asserted interrupts.
    UARTIntClear(UART0_BASE, ulStatus);

	// ѭ���ȴ����뻺�����е�����
    while(UARTCharsAvail(UART0_BASE)){
		// ��ȡ�������е�һ���ֽ�
		#if END_POINT_TX>0	//����Ϊ���ݷ��Ͷ�
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
* ����������������
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


