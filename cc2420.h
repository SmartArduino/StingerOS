#ifndef _CC2420_H_
#define	_CC2420_H_

//以下定义15个命令寄存器
#define SNOP     0X00    //空操作，仅为读取状态寄存器
#define SXOSCON  0X01    //开启晶体振荡器（设置XOSC16M=0,BIAS_PD=0)
#define STXCAL   0X02    //使能和校准频率合成器
#define SRXON    0X03    //RX使能
#define STXON    0X04    //TX使能
#define STXONCCA 0X05    //
#define SRFOFF   0X06    //关闭RX/TX和频率合成器
#define SXOSCOFF 0X07    //关闭晶体振荡器和RF
#define SFLUSHRX 0X08    //清楚接收FIFO缓存
#define SFLUSHTX 0X09    //清楚发送FIFO缓存
#define SACK     0X0A    //发送确认帧，pending域清0
#define SACKPEND 0X0B    //发送确认帧，pending域置1
#define SRXDEC   0X0C    //启动RXFIFO在线解码/认证
#define STXENC   0X0D    //启动TXFIFO在线编码/认证
#define SAES     0X0E    //AES编码

//以下定义33个16位的配置寄存器
#define MAIN      0X10    //主控制寄存器
#define MDMCTRL0  0X11    //调制解调控制寄存器0
#define MDMCTRL1  0X12    //调制解调控制寄存器1
#define RSSI      0X13    //RSSI和CCA状态控制寄存器
#define SYNCWORD  0X14    //同步字i控制寄存器
#define TXCTRL    0X15    //发送控制寄存器
#define RXCTRL0   0X16    //接收控制寄存器0
#define RXCTRL1   0X17    //接收控制寄存器1
#define FSCTRL    0X18    //频率合成器控制与状态寄存器
#define SECCTRL0  0X19    //安全控制寄存器0
#define SECCTRL1  0X1A    //安全控制寄存器1
#define BATTMON   0X1B    //电池监测控制寄存器
#define IOCFG0    0X1C    //I/O配置寄存器0
#define IOCFG1    0X1D    //I/O配置寄存器1
#define MANFIDL   0X1E    //厂商ID号低16位，只读
#define MANFIDH   0X1F    //厂商ID号高16位，只读
#define FSMTC     0X20    //有限状态机定时常数寄存器
#define MANAND    0X21    //人工信号与代理寄存器
#define MANOR     0X22    //人工信号或代理寄存器
#define AGCCTRL   0X23    //AGC控制寄存器
#define FSMSTATE  0X2C    //有限状态机信息寄存器，只读
#define RESERVED  0X30    //保留
#define TXFIFO    0X3E    //TXFIFO寄存器
#define RXFIFO    0X3F    //RXFIFO寄存器

//以下寄存器只用于测试目的
#define AGCTST0   0X24
#define AGCTST1   0X25
#define AGCTST2   0X26
#define FSTST0    0X27
#define FSTST1    0X28
#define FSTST2    0X29
#define FSTST3    0X2A
#define RXBPFTST  0X2B
#define ADCTST    0X2D
#define DACTST    0X2E
#define TOPTST    0X2F

//定义状态寄存器中各个位
#define XOSC16M_STABLE 6   //晶振是否稳定运行
#define TX_UNDERFLOW   5   //发送TXFIFO是否发生下溢
#define ENC_BUSY       4   //编码模块是否处于忙状态
#define TX_ACTIVE      3   //射频发送是否处于活动状态
#define LOCK           2   //频率合成器PLL是否处于锁存状态
#define RSSI_VALID     1   //RSSI值是否有效

#define TXLEN	20

unsigned char read_status(unsigned long port, 
						  unsigned char si, 
						  unsigned char so, 
						  unsigned char cs, 
						  unsigned char sclk);
void write_register(unsigned long val, 
					unsigned long port, 
					unsigned char si, 
					unsigned char cs, 
					unsigned char sclk);
unsigned short read_register(unsigned char val, 
							 unsigned long port, 
							 unsigned char si, 
							 unsigned char so, 
							 unsigned char cs, 
							 unsigned char sclk);
unsigned char write_ram(unsigned char addrH,
						unsigned char addrL,
						unsigned char dataNum,
						unsigned char *pData,
						unsigned long port,
						unsigned char si,
						unsigned char so,
						unsigned char cs,
						unsigned char sclk);
unsigned char read_ram(unsigned char addrH,
					   unsigned char addrL,
						unsigned char dataNum,
						unsigned char *pData,
						unsigned long port,
						unsigned char si,
						unsigned char so,
						unsigned char cs,
						unsigned char sclk);
unsigned char write_config_word(unsigned char addr,
								unsigned char dataH,
								unsigned char dataL,
								unsigned long port, 
								unsigned char si,
								unsigned char so, 
								unsigned char cs, 
								unsigned char sclk);

void cc2420_configuration(unsigned long portVREGEN,
						  unsigned long portCC2430,
						  unsigned char si,
						  unsigned char so,
						  unsigned char cs,
						  unsigned char sclk,
						  unsigned char pinVREGEN);
void powerdown_to_idle_mode(unsigned long port,
							unsigned char si,
							unsigned char so,
							unsigned char cs,
							unsigned char sclk);
void power_off(unsigned long port, unsigned char pinVREGEN);
void power_down_mode(unsigned long portVREGEN,
					 unsigned long portCC2430,
					 unsigned char si,
					 unsigned char so,
					 unsigned char cs,
					 unsigned char sclk, 
					 unsigned char pinVREGEN);
unsigned char write_command(unsigned char command,
							unsigned long port,
							unsigned char si,
							unsigned char so,
							unsigned char cs,
							unsigned char sclk);
void all_status_idle(unsigned long port,
					unsigned char si,
					unsigned char so,
					unsigned char cs,
					unsigned char sclk);
unsigned char write_txfifo(unsigned char num,
			 			   unsigned char *pData,
			 			   unsigned long port,
			 			   unsigned char si,
			 			   unsigned char so,
			 			   unsigned char cs,
			 			   unsigned char sclk);
unsigned char read_rxfifo(unsigned char num,
			 			  unsigned char *pData,
			 			  unsigned long port,
			 			  unsigned char si,
			 			  unsigned char so,
			 			  unsigned char cs,
			 			  unsigned char sclk);
void send(unsigned long port,
		  unsigned char si,
		  unsigned char so,
		  unsigned char cs,
		  unsigned char sclk);
void receive(unsigned long port,
			 unsigned char si,
			 unsigned char so,
			 unsigned char cs,
			 unsigned char sclk);
void write_rxfifo(unsigned char num,
			 	  unsigned char *pData,
			 	  unsigned long port,
			 	  unsigned char si,
			 	  unsigned char so,
			 	  unsigned char cs,
			 	  unsigned char sclk);
unsigned char cc2420_spi(unsigned char addr,
						 unsigned long port,
			 			 unsigned char si,
			 			 unsigned char so,
			 			 unsigned char sclk);

#endif
