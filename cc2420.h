#ifndef _CC2420_H_
#define	_CC2420_H_

//���¶���15������Ĵ���
#define SNOP     0X00    //�ղ�������Ϊ��ȡ״̬�Ĵ���
#define SXOSCON  0X01    //������������������XOSC16M=0,BIAS_PD=0)
#define STXCAL   0X02    //ʹ�ܺ�У׼Ƶ�ʺϳ���
#define SRXON    0X03    //RXʹ��
#define STXON    0X04    //TXʹ��
#define STXONCCA 0X05    //
#define SRFOFF   0X06    //�ر�RX/TX��Ƶ�ʺϳ���
#define SXOSCOFF 0X07    //�رվ���������RF
#define SFLUSHRX 0X08    //�������FIFO����
#define SFLUSHTX 0X09    //�������FIFO����
#define SACK     0X0A    //����ȷ��֡��pending����0
#define SACKPEND 0X0B    //����ȷ��֡��pending����1
#define SRXDEC   0X0C    //����RXFIFO���߽���/��֤
#define STXENC   0X0D    //����TXFIFO���߱���/��֤
#define SAES     0X0E    //AES����

//���¶���33��16λ�����üĴ���
#define MAIN      0X10    //�����ƼĴ���
#define MDMCTRL0  0X11    //���ƽ�����ƼĴ���0
#define MDMCTRL1  0X12    //���ƽ�����ƼĴ���1
#define RSSI      0X13    //RSSI��CCA״̬���ƼĴ���
#define SYNCWORD  0X14    //ͬ����i���ƼĴ���
#define TXCTRL    0X15    //���Ϳ��ƼĴ���
#define RXCTRL0   0X16    //���տ��ƼĴ���0
#define RXCTRL1   0X17    //���տ��ƼĴ���1
#define FSCTRL    0X18    //Ƶ�ʺϳ���������״̬�Ĵ���
#define SECCTRL0  0X19    //��ȫ���ƼĴ���0
#define SECCTRL1  0X1A    //��ȫ���ƼĴ���1
#define BATTMON   0X1B    //��ؼ����ƼĴ���
#define IOCFG0    0X1C    //I/O���üĴ���0
#define IOCFG1    0X1D    //I/O���üĴ���1
#define MANFIDL   0X1E    //����ID�ŵ�16λ��ֻ��
#define MANFIDH   0X1F    //����ID�Ÿ�16λ��ֻ��
#define FSMTC     0X20    //����״̬����ʱ�����Ĵ���
#define MANAND    0X21    //�˹��ź������Ĵ���
#define MANOR     0X22    //�˹��źŻ����Ĵ���
#define AGCCTRL   0X23    //AGC���ƼĴ���
#define FSMSTATE  0X2C    //����״̬����Ϣ�Ĵ�����ֻ��
#define RESERVED  0X30    //����
#define TXFIFO    0X3E    //TXFIFO�Ĵ���
#define RXFIFO    0X3F    //RXFIFO�Ĵ���

//���¼Ĵ���ֻ���ڲ���Ŀ��
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

//����״̬�Ĵ����и���λ
#define XOSC16M_STABLE 6   //�����Ƿ��ȶ�����
#define TX_UNDERFLOW   5   //����TXFIFO�Ƿ�������
#define ENC_BUSY       4   //����ģ���Ƿ���æ״̬
#define TX_ACTIVE      3   //��Ƶ�����Ƿ��ڻ״̬
#define LOCK           2   //Ƶ�ʺϳ���PLL�Ƿ�������״̬
#define RSSI_VALID     1   //RSSIֵ�Ƿ���Ч

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
