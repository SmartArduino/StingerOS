#include "includes.h"
#include "user.h"
#include "stinger.h"
#include "msg.h"
#include "stack.h"
#include "cc2420.h"
#include "common.h"
#include "macro.h"
#include "random.h"
#include "hal.h"

extern unsigned char panID[2];
extern unsigned char shortAddr[2];
extern unsigned char serialNum;

#if	END_POINT_TX > 0 
	//unsigned char dataTX[TXLEN];
	// long型的变量，用作标志判断
	// 总共可做32个标志
	// 第0位：保留
	// 第1位：发送数据测试位，1为发送数据，0为不发送
	extern unsigned long flag;
	//unsigned char dataRAM[TXLEN];
#endif

void user_process_a(void){
	//UINT8 *msg = "Message sending from process A!";

	//UARTprintf("User process A is running!\n");
	/**
	 **	Note:The second parameter of following function is process ID which
	 ** belong to the process calling msg_pending().
	 **/
	//msg_pending(10, procIdA);

/*	unsigned char err, *buff;
	zb_packet_t *zbPacket;
		
	#if END_POINT_TX>0	//发送端数据定义
		unsigned long rand=0;
		int j=0;
		unsigned char fcf[2]={0x01, 0x88};
		unsigned char srcPanID[2]={0x11, 0x11};
		unsigned char srcID[2]={0x11, 0x11};
		unsigned char *payload;
		int i=0;
	#endif

/////////////////////////////////////////////////////////
	#if	END_POINT_TX>0	//配置为数据发送端
		zb_rf_configuration();
	
		zbPacket=zb_packet_create(&err);
		zbPacket->num=TXLEN;

		//帧控制域,无安全，确认要求，数据帧，包含16位的目标地址，源地址，目标PAN标识符，源PAN标识符
		//(zbPacket->fcf)[0]=0x01;		//低字节
		//(zbPacket->fcf)[1]=0x88;		//高字节
		zb_set_fcf(zbPacket, fcf);
		//zbPacket->seq=

		//目的PAN标识符
		(zbPacket->desPanID)[0]=0x45;
		(zbPacket->desPanID)[1]=0x23;

		//目的地址
		(zbPacket->desID)[0]=0x73;
		(zbPacket->desID)[1]=0x15;
		//(zbPacket->srcPanID)[0]=
		//(zbPacket->srcPanID)[1]=
		//(zbPacket->srcID)[0]=
		//(zbPacket->srcID)[1]=
		zbPacket->type=COMMAND_REQUEST;
		zbPacket->sizeOfPayload=zbPacket->num-14;

		buff=zb_buffer_get(zbPacket->num, &err);
		if(err){
			UARTprintf("Error ocurr when get buffer!\n");
		}

		payload=zb_buffer_get(8, &err);
		if(err){
			UARTprintf("Error ocurr when get buffer!\n");
		}

		//初始化payload
		for(i=0; i<8; i++){
			payload[i]=i;
		}

		zbPacket->payload=payload;

		UARTprintf("UART0 initializing...\n");
		j=0;
		while(1){
			//UARTprintf("This is a test\n");
			
			rand=random_number()%5;		//随机选择发射芯片
			switch(rand){
				case 0:
					srcPanID[0]=0x11;
					srcPanID[1]=0x11;
					zb_set_source_pan_id(zbPacket, srcPanID);
					srcID[0]=0x11;
					srcID[1]=0x11;
					zb_set_source_id(zbPacket, srcID);
					break;
				case 1:		
					srcPanID[0]=0x22;	//源PAN标识符
					srcPanID[1]=0x22;
					zb_set_source_pan_id(zbPacket, srcPanID);
					srcID[0]=0x22;		//源地址
					srcID[1]=0x22;
					zb_set_source_id(zbPacket, srcID);
					break;
				case 2:
					srcPanID[0]=0x33;	//源PAN标识符
					srcPanID[1]=0x33;
					zb_set_source_pan_id(zbPacket, srcPanID);
					srcID[0]=0x33;		//源地址
					srcID[1]=0x33;
					zb_set_source_id(zbPacket, srcID);
					break;
				case 3:
					srcPanID[0]=0x44;	//源PAN标识符
					srcPanID[1]=0x44;
					zb_set_source_pan_id(zbPacket, srcPanID);
					srcID[0]=0x44;		//源地址
					srcID[1]=0x44;
					zb_set_source_id(zbPacket, srcID);
					break;
				case 4:
					srcPanID[0]=0x55;	//源PAN标识符
					srcPanID[1]=0x55;
					zb_set_source_pan_id(zbPacket, srcPanID);
					srcID[0]=0x55;		//源地址
					srcID[1]=0x55;
					zb_set_source_id(zbPacket, srcID);
					break;
				default:
					UARTprintf("Error in random!\n");
					break;
			}

			if(j == 10){
				if(flag & SEND_DATA){
					j=0;
					flag &= ~SEND_DATA;
				}
				continue;
			}
			j++;

			// cc2420进入发射前空闲状态
			zb_cc2420_status_idle();

			zbPacket->seq=serialNum;
			serialNum++;
			if(serialNum == 250){
				serialNum = 0;
			}

			zb_packet_send(zbPacket, buff, rand);

			for(i=0; i<20; i++){
				delay(1000);
				delay(1000);
				delay(1000);
			}
		}							   
	
		//zb_packet_release(zbPacket);
		//zb_buffer_release(buff);

	#endif
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
	#if END_POINT_RX>0	//配置为数据接收端
		zb_receive_configuration();	
		write_ram(0x6a, 0x02, 2, shortAddr, GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);
		write_ram(0x68, 0x02, 2, panID, GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);

		all_status_idle(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);
		receive(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);

		UARTprintf("UART0 initializing...\n");
		write_command(SFLUSHRX, GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);

		zbPacket=zb_packet_create(&err);
		zbPacket->num=TXLEN;
		buff=zb_buffer_get(zbPacket->num, &err);

		while(1){
			if(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_7)){
				if(!zb_packet_receive(zbPacket, buff)){
					UARTprintf("CRC check failed!\n");
				}else{
					UARTprintf("%d\t0x%02x%02x\t%d\t0x%02x%02x\t0x%02x%02x\t0x%02x%02x\t0x%02x%02x\t%d\t%d\n", 
								zbPacket->num,
								(zbPacket->fcf)[1],
								(zbPacket->fcf)[0],
								zbPacket->seq,
								(zbPacket->desPanID)[1],
								(zbPacket->desPanID)[0],
								(zbPacket->desID)[1],
								(zbPacket->desID)[0],
								(zbPacket->srcPanID)[1],
								(zbPacket->srcPanID)[0],
								(zbPacket->srcID)[1],
								(zbPacket->srcID)[0],
								zbPacket->type,
								zbPacket->sizeOfPayload);
					//UARTprintf("CRC check passed\n");
				}


				delay(1000);
				delay(1000);
				delay(1000);
			}
		}

		//zb_packet_release(zbPacket);
		//zb_buffer_release(buff);
	#endif				*/
	UARTprintf("User process A is running!\n");
}

void user_process_b(void){
	//UINT8 *msg = "Message sending from process A!";
	//msg_t *newMsg;

	UARTprintf("User process B is running!\n");
/*	if(msg_create(&newMsg)){
		UARTprintf("Message created successfully!\n");
	}			 */
	/** 
	 ** Note:The third parameter of msg_send() is process 
	 ** ID that you want to send messages to.
	 **/
	//msg_send(newMsg, msg, procIdA);
}

void user_process_init(void){
	//unsigned char err=0;
	UINT32 *stkA, *stkB;
	process_control_block_t *ret;

	stkA=(unsigned long *)os_mem_alloc(PROC_STACK_SIZE);
	stkB=(unsigned long *)os_mem_alloc(PROC_STACK_SIZE);

	

	process_create(procIdA, 1, ret, user_process_a, stkA);
	process_create(procIdB, 2, ret, user_process_b, stkB);
	//current=idle->next;
}
