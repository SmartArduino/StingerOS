#ifndef MACRO_H
#define MACRO_H

#include "hw_types.h"
#include "gpio.h"
#include "hw_memmap.h"

/***************************************
* 有用的宏，电路管脚改动，则改动这些宏即可
***************************************/
#define CHIP_1_PORT			GPIO_PORTA_BASE
#define CHIP_1_VREGEN_PORT	GPIO_PORTG_BASE
#define CHIP_1_SI			GPIO_PIN_5
#define CHIP_1_SO			GPIO_PIN_4
#define CHIP_1_CS			GPIO_PIN_3
#define CHIP_1_SCLK			GPIO_PIN_2
#define CHIP_1_VREGEN_PIN	GPIO_PIN_6
#define CHIP_1_FIFOP_PORT	GPIO_PORTF_BASE
#define CHIP_1_FIFOP_PIN	GPIO_PIN_1
#define CHIP_1_FIFO_PORT	GPIO_PORTG_BASE
#define CHIP_1_FIFO_PIN		GPIO_PIN_7		

#define CHIP_2_PORT			GPIO_PORTC_BASE
#define CHIP_2_VREGEN_PORT	GPIO_PORTG_BASE
#define CHIP_2_SI			GPIO_PIN_7
#define CHIP_2_SO			GPIO_PIN_6
#define CHIP_2_CS			GPIO_PIN_5
#define CHIP_2_SCLK			GPIO_PIN_4						
#define CHIP_2_VREGEN_PIN	GPIO_PIN_2
#define CHIP_2_FIFOP_PORT	GPIO_PORTF_BASE
#define CHIP_2_FIFOP_PIN	GPIO_PIN_2
#define CHIP_2_FIFO_PORT	GPIO_PORTG_BASE
#define CHIP_2_FIFO_PIN		GPIO_PIN_3	

#define CHIP_3_PORT			GPIO_PORTE_BASE
#define CHIP_3_VREGEN_PORT	GPIO_PORTE_BASE
#define CHIP_3_SI			GPIO_PIN_3
#define CHIP_3_SO			GPIO_PIN_2
#define CHIP_3_CS			GPIO_PIN_1
#define CHIP_3_SCLK			GPIO_PIN_0						
#define CHIP_3_VREGEN_PIN	GPIO_PIN_4
#define CHIP_3_FIFOP_PORT	GPIO_PORTF_BASE
#define CHIP_3_FIFOP_PIN	GPIO_PIN_3
#define CHIP_3_FIFO_PORT	GPIO_PORTE_BASE
#define CHIP_3_FIFO_PIN		GPIO_PIN_5	

#define CHIP_4_PORT			GPIO_PORTB_BASE
#define CHIP_4_VREGEN_PORT	GPIO_PORTB_BASE
#define CHIP_4_SI			GPIO_PIN_3
#define CHIP_4_SO			GPIO_PIN_2
#define CHIP_4_CS			GPIO_PIN_1
#define CHIP_4_SCLK			GPIO_PIN_0						
#define CHIP_4_VREGEN_PIN	GPIO_PIN_6
#define CHIP_4_FIFOP_PORT	GPIO_PORTG_BASE
#define CHIP_4_FIFOP_PIN	GPIO_PIN_0
#define CHIP_4_FIFO_PORT	GPIO_PORTF_BASE
#define CHIP_4_FIFO_PIN		GPIO_PIN_0	

#define CHIP_5_PORT			GPIO_PORTD_BASE
#define CHIP_5_VREGEN_PORT	GPIO_PORTD_BASE
#define CHIP_5_SI			GPIO_PIN_3
#define CHIP_5_SO			GPIO_PIN_2
#define CHIP_5_CS			GPIO_PIN_1
#define CHIP_5_SCLK			GPIO_PIN_0						
#define CHIP_5_VREGEN_PIN	GPIO_PIN_7
#define CHIP_5_FIFOP_PORT	GPIO_PORTG_BASE
#define CHIP_5_FIFOP_PIN	GPIO_PIN_1
#define CHIP_5_FIFO_PORT	GPIO_PORTD_BASE
#define CHIP_5_FIFO_PIN		GPIO_PIN_4	

/***************************************
* 配置为发射端或接收端
***************************************/
#define END_POINT_RX	0
#define END_POINT_TX	1

#define SEND_DATA		0x00000002

#endif

