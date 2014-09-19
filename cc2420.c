/*******************************************
* cc2420功能代码
* Nicholas3388
* 2011.11.15
********************************************/
#include "cc2420.h"
#include "includes.h"
#include "common.h"

extern unsigned char dataRAM[];
/*******************************************
* 函数功能：读状态寄存器
* 输入参数：port：GPIO口
*			si：数据输入
*			so：数据输出
*			cs：片选
*			sclk：时钟输入
* 输出参数：无
* 返 回 值：状态寄存器的值
********************************************/
unsigned char read_status(unsigned long port, 
						  unsigned char si, 
						  unsigned char so, 
						  unsigned char cs, 
						  unsigned char sclk){
	unsigned char ret=0;
	int i=0;

	//ret = read_register(val, port, si, so, cs, sclk);
	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);

	for(i=0; i<8; i++){
		GPIOPinWrite(port, si, ~si);

		GPIOPinWrite(port, sclk, sclk);	//上升沿数据输入
		GPIOPinWrite(port, sclk, ~sclk);
	}
	for(i=0; i<8; i++){
		GPIOPinWrite(port, sclk, sclk);	//上升沿数据输出
		if(GPIOPinRead(port, so)){
			ret |= (1<<(7-i));
		}
		GPIOPinWrite(port, sclk, ~sclk);
	}

	GPIOPinWrite(port, cs, cs);

	return (ret);	//高8位才是寄存器状态值？ 
}

/*******************************************
* 函数功能：写指令
* 输入参数：command：命令字
*			port：GPIO口
*			si：数据输入
*			so：数据输出
*			cs：片选
*			sclk：时钟输入
* 输出参数：无
* 返 回 值：状态寄存器的值
********************************************/
unsigned char write_command(unsigned char command,
							unsigned long port,
							unsigned char si,
							unsigned char so,
							unsigned char cs,
							unsigned char sclk){
	unsigned char stat=0;

	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);
	stat=cc2420_spi(command, port, si, so, sclk);
	GPIOPinWrite(port, cs, cs);

	return stat;
}

/*******************************************
* 函数功能：写寄存器
* 输入参数：val：向寄存器写入的数值
* 			port：GPIO口
*			si：数据输入管脚
*			cs：片选
*			sclk：时钟输入管脚
* 返 回 值：无
********************************************/
void write_register(unsigned long val, 
					unsigned long port, 
					unsigned char si, 
					unsigned char cs, 
					unsigned char sclk){
	int i=0;
	unsigned char temp=0;

	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);
	for(i=0; i<24; i++){		
		temp = (val>>(23-i))&0x1;
		if(temp){
			GPIOPinWrite(port, si, si);
		}else{
			GPIOPinWrite(port, si, ~si);
		}

		GPIOPinWrite(port, sclk, sclk);	//上升沿数据输入
		GPIOPinWrite(port, sclk, ~sclk);
	}

	GPIOPinWrite(port, cs, cs);
}

/*******************************************
* 函数功能：读寄存器
* 输入参数：val：向寄存器写入的数值
* 			port：GPIO口
*			si：数据输入管脚
*			so：数据输出
*			cs：片选
*			sclk：时钟输入管脚
* 返 回 值：读出的数据
********************************************/
unsigned short read_register(unsigned char val, 
							 unsigned long port, 
							 unsigned char si, 
							 unsigned char so, 
							 unsigned char cs, 
							 unsigned char sclk){
	unsigned short ret=0, temp=0;
	int i=0;

	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);

	val |= 0x40;

	for(i=0; i<8; i++){
		temp = (val>>(7-i))&0x1;
		if(temp){
			GPIOPinWrite(port, si, si);
		}else{
			GPIOPinWrite(port, si, ~si);
		}

		GPIOPinWrite(port, sclk, sclk);	//上升沿数据输入
		GPIOPinWrite(port, sclk, ~sclk);
	}
	for(i=0; i<16; i++){
		GPIOPinWrite(port, sclk, sclk);	//上升沿数据输出
		if(GPIOPinRead(port, so)){
			ret |= (1<<(15-i));
		}
		GPIOPinWrite(port, sclk, ~sclk);
	}

	GPIOPinWrite(port, cs, cs);

	return ret;
}

/*******************************************
* 函数功能：向寄存器写入一个字
* 输入参数：addr：寄存器地址
*			dataH：数据高8位
*			dataL：数据低8位
* 			port：GPIO口
*			si：数据输入管脚
*			so：数据输出管脚
*			cs：片选
*			sclk：时钟输入管脚
* 返 回 值：无
********************************************/
unsigned char write_config_word(unsigned char addr,
								unsigned char dataH,
								unsigned char dataL,
								unsigned long port, 
								unsigned char si,
								unsigned char so, 
								unsigned char cs, 
								unsigned char sclk){
	unsigned char stat;
	unsigned long val=0;

	val |= addr<<16;
	val |= dataH<<8;
	val |= dataL;
	write_register(val, port, si, cs, sclk);
	stat = read_register(val, port, si, so, cs, sclk);
	return stat;
}

/*******************************************
* 函数功能：向寄存器读一个字
* 输入参数：addr：寄存器地址
* 			port：GPIO口
*			si：数据输入管脚
*			so：数据输出管脚
*			cs：片选
*			sclk：时钟输入管脚
* 返 回 值：无
********************************************/
unsigned short read_config_word(unsigned char addr,
								unsigned long port, 
								unsigned char si,
								unsigned char so, 
								unsigned char cs, 
								unsigned char sclk){
	unsigned short ret=0;


	return ret;
}

/*******************************************
* 函数功能：写RAM 
* 输入参数：pData:存储数据的缓存首地址；
*          dataNum:要写入的字节数
*		   addrH：在选定的RAM存储块中确定RAM地址
*		   addrL：选择RAM存储块
*		   cs: 片选
*		   si: 数据输入
*		   so: 数据输出
*		   sclk: 时钟输入
* 输出参数：无
* 返 回 值：状态寄存器的值
********************************************/
unsigned char write_ram(unsigned char addrH,
						unsigned char addrL,
						unsigned char dataNum,
						unsigned char *pData,
						unsigned long port,
						unsigned char si,
						unsigned char so,
						unsigned char cs,
						unsigned char sclk){
	unsigned char stat;
	unsigned short addr=0, temp=0;
	int i=0, j=0;

	addrH |= 0x80;		//访问RAM位置1
	addrL <<= 6;
	addrL &= 0XDF;      //读写操作位置0，表示写操作
	addr = ((addrH<<8)|addrL);

	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);

	for(i=0; i<16; i++){	//操作数输入
		temp = (addr>>(15-i))&0x1;
		if(temp){
			GPIOPinWrite(port, si, si);
		}else{
			GPIOPinWrite(port, si, ~si);
		}
		GPIOPinWrite(port, sclk, sclk);	//上升沿数据输入
		GPIOPinWrite(port, sclk, ~sclk);
	}

	for(i=0; i<dataNum; i++){
		for(j=0; j<8; j++){
			if((pData[i]>>(7-j))&0x1){
				GPIOPinWrite(port, si, si);
			}else{
				GPIOPinWrite(port, si, ~si);
			}
			GPIOPinWrite(port, sclk, sclk);	//上升沿数据输入
			GPIOPinWrite(port, sclk, ~sclk);	
		}
	}

	GPIOPinWrite(port, cs, cs);	//cs拉高，结束数据输入

	stat = read_status(port, si, so, cs, sclk);
	return stat;
}

/*******************************************
* 函数功能：读RAM 
* 输入参数：pData:存储数据的缓存首地址；
*		   addrH：在选定的RAM存储块中确定RAM地址
*		   addrL：选择RAM存储块
*		   cs: 片选
*		   si: 数据输入
*		   so: 数据输出
*		   sclk: 时钟输入
* 输出参数：无
* 返 回 值：状态寄存器的值
********************************************/
unsigned char read_ram(unsigned char addrH,
					   unsigned char addrL,
						unsigned char dataNum,
						unsigned char *pData,
						unsigned long port,
						unsigned char si,
						unsigned char so,
						unsigned char cs,
						unsigned char sclk){
	unsigned char stat=0;
	unsigned short temp=0, addr=0;
	int i=0,j=0;

	addrH |= 0x80;		//访问RAM位置1
	addrL <<= 6;
	addrL |= 0X20;      //读写操作位置1，表示读操作
	addr = ((addrH<<8)|addrL);

	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);
	for(i=0; i<16; i++){	//操作数输入
		temp = (addr>>(15-i))&0x1;
		if(temp){
			GPIOPinWrite(port, si, si);
		}else{
			GPIOPinWrite(port, si, ~si);
		}
		GPIOPinWrite(port, sclk, sclk);	//上升沿数据输入
		GPIOPinWrite(port, sclk, ~sclk);
	}

	for(j=0; j<dataNum; j++){	//ram数据输入
		for(i=0; i<8; i++){
			GPIOPinWrite(port, sclk, sclk);	//上升沿数据输入
			if(GPIOPinRead(port, so)){
				pData[j] |= (1<<(7-i));
			}
			GPIOPinWrite(port, sclk, ~sclk);	
		}
	}
	GPIOPinWrite(port, cs, cs);	//cs拉高，结束数据输入
	return stat;
}

/*******************************************
* 函数功能：配置cc2420寄存器
* 输入参数：无
* 返 回 值：无
********************************************/
void cc2420_configuration(unsigned long portVREGEN,
						  unsigned long portCC2430,
						  unsigned char si,
						  unsigned char so,
						  unsigned char cs,
						  unsigned char sclk,
						  unsigned char pinVREGEN){
	power_off(portVREGEN, pinVREGEN);	//第一片cc2420
	power_down_mode(portVREGEN, portCC2430, si, so, cs, sclk, pinVREGEN);	//在power down模式下设置配置寄存器
	
	write_config_word(MDMCTRL0,	//地址认证使能，自动应答不使能 
					  0X0A, 
					  0XE2, 
					  portCC2430, 
					  si, 
					  so, 
					  cs, 
					  sclk);

	write_config_word(MDMCTRL1,	//地址认证使能，自动应答不使能 
					  0X05, 
					  0X00, 
					  portCC2430, 
					  si, 
					  so, 
					  cs, 
					  sclk);

	write_config_word(FSCTRL,	//2.45GHz
					  0X41, 
					  0X92, 
					  portCC2430, 
					  si, 
					  so, 
					  cs, 
					  sclk);

	write_config_word(SECCTRL0,	//安全模式不使能
					  0X01, 
					  0XE4, 
					  portCC2430, 
					  si, 
					  so, 
					  cs, 
					  sclk);

	write_config_word(IOCFG0,	//RXFIFO的极限值
					  0X00, 
					  TXLEN, 
					  portCC2430, 
					  si, 
					  so, 
					  cs, 
					  sclk);	  
}

/*******************************************
* 函数功能：从掉电模式转换到空闲模式 
* 输入参数：无
* 输出参数：无
* 返 回 值：无
********************************************/
void powerdown_to_idle_mode(unsigned long port,
							unsigned char si,
							unsigned char so,
							unsigned char cs,
							unsigned char sclk){
	unsigned char stat=0;

	stat=write_command(SXOSCON, port, si, so, cs, sclk);
	while(1){
		stat=read_status(port, si, so, cs, sclk);
		if((stat&(1<<XOSC16M_STABLE))!=0){
			break;
		}
	}
}

/*******************************************
* 函数功能：关闭电压校准器模式 
* 输入参数：pinVREGEN：某片cc2420的VREG_EN
*		    port：GPIO口
* 输出参数：无
* 返 回 值：无
********************************************/
void power_off(unsigned long port, unsigned char pinVREGEN){
	GPIOPinWrite(port, pinVREGEN, ~pinVREGEN);
}

/*******************************************
* 函数功能：关闭电压校准器模式 
* 输入参数：pinVREGEN：谋片cc2420的VREG_EN
*		    port：GPIO口
*	        si：数据输入
*		    so：数据输出
* 		    cs：片选端
* 		    sclk：时钟输入
* 输出参数：无
* 返 回 值：无
********************************************/
void power_down_mode(unsigned long portVREGEN,
					 unsigned long portCC2430,
					 unsigned char si,
					 unsigned char so,
					 unsigned char cs,
					 unsigned char sclk, 
					 unsigned char pinVREGEN){
	GPIOPinWrite(portVREGEN, pinVREGEN, pinVREGEN);
	delay(1000);

	write_config_word(MAIN, 0x78, 0x00, portCC2430, si, so, cs, sclk);
	write_config_word(MAIN, 0xf8, 0x00, portCC2430, si, so, cs, sclk);
}

/*******************************************
* 函数功能：从其他任何模式转换到空闲模式 
* 输入参数：port：GPIO口		   
*	        si：数据输入
*		    so：数据输出
* 		    cs：片选端
* 		    sclk：时钟输入
* 输出参数：无
* 返 回 值：无
********************************************/
void all_status_idle(unsigned long port,
					unsigned char si,
					unsigned char so,
					unsigned char cs,
					unsigned char sclk){

	write_command(SRFOFF, port, si, so, cs, sclk);

}

/*******************************************
* 函数功能：写TXFIFO 
* 输入参数：num：数据字节数
*			pData：指向保存数据的缓存
*			port：GPIO口		   
*	        si：数据输入
*		    so：数据输出
* 		    cs：片选端
* 		    sclk：时钟输入
* 输出参数：无
* 返 回 值：无
********************************************/
unsigned char write_txfifo(unsigned char num,
			 			   unsigned char *pData,
			 			   unsigned long port,
			 			   unsigned char si,
			 			   unsigned char so,
			 			   unsigned char cs,
			 			   unsigned char sclk){
	unsigned char stat=0, addr=0;
	int i=0;

	addr = TXFIFO;
	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);

	GPIOPinWrite(port, cs, ~cs);

	cc2420_spi(addr, port, si, so, sclk);

	for(i=0; i<num; i++){
		stat=0;
		stat=cc2420_spi(pData[i], port, si, so, sclk);
		if((stat&(1<<TX_UNDERFLOW))!=0x00){         //发生下溢
			write_command(SFLUSHTX, port, si, so, cs, sclk);
		}
	}

	GPIOPinWrite(port, cs, cs);	//cs管脚拉高，结束数据传输		  /* */

/*	stat=write_ram(TXFIFO, 0x00, num, pData, port, si, so, cs, sclk);
	if((stat&(1<<TX_UNDERFLOW))!=0x00){
		 write_command(SFLUSHTX, port, si, so, cs, sclk);
	}*/

	return stat;
}

/*******************************************
* 函数功能：读RXFIFO 
* 输入参数：num：数据字节数
*			pData：指向保存数据的缓存
*			port：GPIO口		   
*	        si：数据输入
*		    so：数据输出
* 		    cs：片选端
* 		    sclk：时钟输入
* 输出参数：无
* 返 回 值：无
********************************************/
unsigned char read_rxfifo(unsigned char num,
			 			  unsigned char *pData,
			 			  unsigned long port,
			 			  unsigned char si,
			 			  unsigned char so,
			 			  unsigned char cs,
			 			  unsigned char sclk){
	unsigned char stat=0, addr=0/*, n=num*/;
	int i=0;

	addr = RXFIFO | 0x40;

	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);
	
	stat=cc2420_spi(addr, port, si, so, sclk);

	for(i=0; i<num; i++){
		pData[i]=cc2420_spi(0xff, port, si, so, sclk);
	}
	GPIOPinWrite(port, cs, cs);/**/

	//stat=read_ram(RXFIFO, 0x01, num, pData, port, si, so, cs, sclk);

	return stat;
}

/*******************************************
* 函数功能：写RXFIFO 
* 输入参数：num：数据字节数
*			pData：指向保存数据的缓存
*			port：GPIO口		   
*	        si：数据输入
*		    so：数据输出
* 		    cs：片选端
* 		    sclk：时钟输入
* 输出参数：无
* 返 回 值：无
********************************************/
void write_rxfifo(unsigned char num,
			 	  unsigned char *pData,
			 	  unsigned long port,
			 	  unsigned char si,
			 	  unsigned char so,
			 	  unsigned char cs,
			 	  unsigned char sclk){
	unsigned char addr=RXFIFO;
	int i=0,j=0;

	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);
	for(i=0; i<8; i++){
		if((addr>>(7-i))&0x1){
			GPIOPinWrite(port, si, si);
		}else{
			GPIOPinWrite(port, si, ~si);
		}
	
		GPIOPinWrite(port, sclk, sclk);	//上升沿数据输入
		GPIOPinWrite(port, sclk, ~sclk);
	}
	for(i=0; i<num; i++){
		for(j=0; j<8; j++){
			if((pData[i]>>(7-j))&0x1){
				GPIOPinWrite(port, si, si);
			}else{
				GPIOPinWrite(port, si, ~si);
			}
			GPIOPinWrite(port, sclk, sclk);	//上升沿数据输入		
			GPIOPinWrite(port, sclk, ~sclk);
		}
	}
	GPIOPinWrite(port, cs, cs);
}

/*******************************************
* 函数功能：数据发送函数 
* 输入参数：port：GPIO口		   
*	        si：数据输入
*		    so：数据输出
* 		    cs：片选端
* 		    sclk：时钟输入
* 输出参数：无
* 返 回 值：无
********************************************/
void send(unsigned long port,
		  unsigned char si,
		  unsigned char so,
		  unsigned char cs,
		  unsigned char sclk){
	
	write_command(STXON, port, si, so, cs, sclk);
	/*if((stat&(1<<TX_UNDERFLOW))!=0x00){
		 write_command(SFLUSHTX, port, si, so, cs, sclk);
	}*/
	delay(1000);
	delay(1000);
	delay(1000);
}

/*******************************************
* 函数功能：数据发送函数 
* 输入参数：port：GPIO口		   
*	        si：数据输入
*		    so：数据输出
* 		    cs：片选端
* 		    sclk：时钟输入
* 输出参数：无
* 返 回 值：无
********************************************/
void receive(unsigned long port,
			 unsigned char si,
			 unsigned char so,
			 unsigned char cs,
			 unsigned char sclk){
	write_command(SRXON, port, si, so, cs, sclk);
}

/*******************************************
* 函数功能：SPI 要在CSn使能时使用 
* 输入参数：port：GPIO口		   
*	        si：数据输入
*		    so：数据输出
* 		    cs：片选端
* 		    sclk：时钟输入
* 输出参数：无
* 返 回 值：无
********************************************/
unsigned char cc2420_spi(unsigned char addr,
						 unsigned long port,
			 			 unsigned char si,
			 			 unsigned char so,
			 			 unsigned char sclk){
	unsigned char ret=0;
	int i=0;

	for(i=0; i<8; i++){
		GPIOPinWrite(port, sclk, ~sclk);
		if((addr>>(7-i))&0x1){
			GPIOPinWrite(port, si, si);
		}else{
			GPIOPinWrite(port, si, ~si);
		}

		GPIOPinWrite(port, sclk, sclk);
		if(GPIOPinRead(port, so)){		//保存寄存器状态值
			ret |= (1<<(7-i));
		}
		GPIOPinWrite(port, sclk, ~sclk);
	}
	return ret;
}

/*******************************************
* 函数功能：cc2420芯片使能 
* 输入参数：port：某片cc2420的GPIO口		   
*	        cs：片选
* 输出参数：无
* 返 回 值：无
********************************************/
void cc2420_cs_enable(unsigned long port,
			 		  unsigned char cs){
	GPIOPinWrite(port, cs, ~cs);	//cs低电平时使能					  
}

/*******************************************
* 函数功能：cc2420芯片禁能 
* 输入参数：port：某片cc2420的GPIO口		   
*	        cs：片选
* 输出参数：无
* 返 回 值：无
********************************************/
void cc2420_cs_disable(unsigned long port,
			 		   unsigned char cs){
	GPIOPinWrite(port, cs, cs);	//cs高电平时禁能
}


