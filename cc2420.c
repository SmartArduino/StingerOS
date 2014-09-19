/*******************************************
* cc2420���ܴ���
* Nicholas3388
* 2011.11.15
********************************************/
#include "cc2420.h"
#include "includes.h"
#include "common.h"

extern unsigned char dataRAM[];
/*******************************************
* �������ܣ���״̬�Ĵ���
* ���������port��GPIO��
*			si����������
*			so���������
*			cs��Ƭѡ
*			sclk��ʱ������
* �����������
* �� �� ֵ��״̬�Ĵ�����ֵ
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

		GPIOPinWrite(port, sclk, sclk);	//��������������
		GPIOPinWrite(port, sclk, ~sclk);
	}
	for(i=0; i<8; i++){
		GPIOPinWrite(port, sclk, sclk);	//�������������
		if(GPIOPinRead(port, so)){
			ret |= (1<<(7-i));
		}
		GPIOPinWrite(port, sclk, ~sclk);
	}

	GPIOPinWrite(port, cs, cs);

	return (ret);	//��8λ���ǼĴ���״ֵ̬�� 
}

/*******************************************
* �������ܣ�дָ��
* ���������command��������
*			port��GPIO��
*			si����������
*			so���������
*			cs��Ƭѡ
*			sclk��ʱ������
* �����������
* �� �� ֵ��״̬�Ĵ�����ֵ
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
* �������ܣ�д�Ĵ���
* ���������val����Ĵ���д�����ֵ
* 			port��GPIO��
*			si����������ܽ�
*			cs��Ƭѡ
*			sclk��ʱ������ܽ�
* �� �� ֵ����
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

		GPIOPinWrite(port, sclk, sclk);	//��������������
		GPIOPinWrite(port, sclk, ~sclk);
	}

	GPIOPinWrite(port, cs, cs);
}

/*******************************************
* �������ܣ����Ĵ���
* ���������val����Ĵ���д�����ֵ
* 			port��GPIO��
*			si����������ܽ�
*			so���������
*			cs��Ƭѡ
*			sclk��ʱ������ܽ�
* �� �� ֵ������������
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

		GPIOPinWrite(port, sclk, sclk);	//��������������
		GPIOPinWrite(port, sclk, ~sclk);
	}
	for(i=0; i<16; i++){
		GPIOPinWrite(port, sclk, sclk);	//�������������
		if(GPIOPinRead(port, so)){
			ret |= (1<<(15-i));
		}
		GPIOPinWrite(port, sclk, ~sclk);
	}

	GPIOPinWrite(port, cs, cs);

	return ret;
}

/*******************************************
* �������ܣ���Ĵ���д��һ����
* ���������addr���Ĵ�����ַ
*			dataH�����ݸ�8λ
*			dataL�����ݵ�8λ
* 			port��GPIO��
*			si����������ܽ�
*			so����������ܽ�
*			cs��Ƭѡ
*			sclk��ʱ������ܽ�
* �� �� ֵ����
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
* �������ܣ���Ĵ�����һ����
* ���������addr���Ĵ�����ַ
* 			port��GPIO��
*			si����������ܽ�
*			so����������ܽ�
*			cs��Ƭѡ
*			sclk��ʱ������ܽ�
* �� �� ֵ����
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
* �������ܣ�дRAM 
* ���������pData:�洢���ݵĻ����׵�ַ��
*          dataNum:Ҫд����ֽ���
*		   addrH����ѡ����RAM�洢����ȷ��RAM��ַ
*		   addrL��ѡ��RAM�洢��
*		   cs: Ƭѡ
*		   si: ��������
*		   so: �������
*		   sclk: ʱ������
* �����������
* �� �� ֵ��״̬�Ĵ�����ֵ
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

	addrH |= 0x80;		//����RAMλ��1
	addrL <<= 6;
	addrL &= 0XDF;      //��д����λ��0����ʾд����
	addr = ((addrH<<8)|addrL);

	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);

	for(i=0; i<16; i++){	//����������
		temp = (addr>>(15-i))&0x1;
		if(temp){
			GPIOPinWrite(port, si, si);
		}else{
			GPIOPinWrite(port, si, ~si);
		}
		GPIOPinWrite(port, sclk, sclk);	//��������������
		GPIOPinWrite(port, sclk, ~sclk);
	}

	for(i=0; i<dataNum; i++){
		for(j=0; j<8; j++){
			if((pData[i]>>(7-j))&0x1){
				GPIOPinWrite(port, si, si);
			}else{
				GPIOPinWrite(port, si, ~si);
			}
			GPIOPinWrite(port, sclk, sclk);	//��������������
			GPIOPinWrite(port, sclk, ~sclk);	
		}
	}

	GPIOPinWrite(port, cs, cs);	//cs���ߣ�������������

	stat = read_status(port, si, so, cs, sclk);
	return stat;
}

/*******************************************
* �������ܣ���RAM 
* ���������pData:�洢���ݵĻ����׵�ַ��
*		   addrH����ѡ����RAM�洢����ȷ��RAM��ַ
*		   addrL��ѡ��RAM�洢��
*		   cs: Ƭѡ
*		   si: ��������
*		   so: �������
*		   sclk: ʱ������
* �����������
* �� �� ֵ��״̬�Ĵ�����ֵ
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

	addrH |= 0x80;		//����RAMλ��1
	addrL <<= 6;
	addrL |= 0X20;      //��д����λ��1����ʾ������
	addr = ((addrH<<8)|addrL);

	GPIOPinWrite(port, cs, cs);
	GPIOPinWrite(port, sclk, ~sclk);
	GPIOPinWrite(port, cs, ~cs);
	for(i=0; i<16; i++){	//����������
		temp = (addr>>(15-i))&0x1;
		if(temp){
			GPIOPinWrite(port, si, si);
		}else{
			GPIOPinWrite(port, si, ~si);
		}
		GPIOPinWrite(port, sclk, sclk);	//��������������
		GPIOPinWrite(port, sclk, ~sclk);
	}

	for(j=0; j<dataNum; j++){	//ram��������
		for(i=0; i<8; i++){
			GPIOPinWrite(port, sclk, sclk);	//��������������
			if(GPIOPinRead(port, so)){
				pData[j] |= (1<<(7-i));
			}
			GPIOPinWrite(port, sclk, ~sclk);	
		}
	}
	GPIOPinWrite(port, cs, cs);	//cs���ߣ�������������
	return stat;
}

/*******************************************
* �������ܣ�����cc2420�Ĵ���
* �����������
* �� �� ֵ����
********************************************/
void cc2420_configuration(unsigned long portVREGEN,
						  unsigned long portCC2430,
						  unsigned char si,
						  unsigned char so,
						  unsigned char cs,
						  unsigned char sclk,
						  unsigned char pinVREGEN){
	power_off(portVREGEN, pinVREGEN);	//��һƬcc2420
	power_down_mode(portVREGEN, portCC2430, si, so, cs, sclk, pinVREGEN);	//��power downģʽ���������üĴ���
	
	write_config_word(MDMCTRL0,	//��ַ��֤ʹ�ܣ��Զ�Ӧ��ʹ�� 
					  0X0A, 
					  0XE2, 
					  portCC2430, 
					  si, 
					  so, 
					  cs, 
					  sclk);

	write_config_word(MDMCTRL1,	//��ַ��֤ʹ�ܣ��Զ�Ӧ��ʹ�� 
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

	write_config_word(SECCTRL0,	//��ȫģʽ��ʹ��
					  0X01, 
					  0XE4, 
					  portCC2430, 
					  si, 
					  so, 
					  cs, 
					  sclk);

	write_config_word(IOCFG0,	//RXFIFO�ļ���ֵ
					  0X00, 
					  TXLEN, 
					  portCC2430, 
					  si, 
					  so, 
					  cs, 
					  sclk);	  
}

/*******************************************
* �������ܣ��ӵ���ģʽת��������ģʽ 
* �����������
* �����������
* �� �� ֵ����
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
* �������ܣ��رյ�ѹУ׼��ģʽ 
* ���������pinVREGEN��ĳƬcc2420��VREG_EN
*		    port��GPIO��
* �����������
* �� �� ֵ����
********************************************/
void power_off(unsigned long port, unsigned char pinVREGEN){
	GPIOPinWrite(port, pinVREGEN, ~pinVREGEN);
}

/*******************************************
* �������ܣ��رյ�ѹУ׼��ģʽ 
* ���������pinVREGEN��ıƬcc2420��VREG_EN
*		    port��GPIO��
*	        si����������
*		    so���������
* 		    cs��Ƭѡ��
* 		    sclk��ʱ������
* �����������
* �� �� ֵ����
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
* �������ܣ��������κ�ģʽת��������ģʽ 
* ���������port��GPIO��		   
*	        si����������
*		    so���������
* 		    cs��Ƭѡ��
* 		    sclk��ʱ������
* �����������
* �� �� ֵ����
********************************************/
void all_status_idle(unsigned long port,
					unsigned char si,
					unsigned char so,
					unsigned char cs,
					unsigned char sclk){

	write_command(SRFOFF, port, si, so, cs, sclk);

}

/*******************************************
* �������ܣ�дTXFIFO 
* ���������num�������ֽ���
*			pData��ָ�򱣴����ݵĻ���
*			port��GPIO��		   
*	        si����������
*		    so���������
* 		    cs��Ƭѡ��
* 		    sclk��ʱ������
* �����������
* �� �� ֵ����
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
		if((stat&(1<<TX_UNDERFLOW))!=0x00){         //��������
			write_command(SFLUSHTX, port, si, so, cs, sclk);
		}
	}

	GPIOPinWrite(port, cs, cs);	//cs�ܽ����ߣ��������ݴ���		  /* */

/*	stat=write_ram(TXFIFO, 0x00, num, pData, port, si, so, cs, sclk);
	if((stat&(1<<TX_UNDERFLOW))!=0x00){
		 write_command(SFLUSHTX, port, si, so, cs, sclk);
	}*/

	return stat;
}

/*******************************************
* �������ܣ���RXFIFO 
* ���������num�������ֽ���
*			pData��ָ�򱣴����ݵĻ���
*			port��GPIO��		   
*	        si����������
*		    so���������
* 		    cs��Ƭѡ��
* 		    sclk��ʱ������
* �����������
* �� �� ֵ����
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
* �������ܣ�дRXFIFO 
* ���������num�������ֽ���
*			pData��ָ�򱣴����ݵĻ���
*			port��GPIO��		   
*	        si����������
*		    so���������
* 		    cs��Ƭѡ��
* 		    sclk��ʱ������
* �����������
* �� �� ֵ����
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
	
		GPIOPinWrite(port, sclk, sclk);	//��������������
		GPIOPinWrite(port, sclk, ~sclk);
	}
	for(i=0; i<num; i++){
		for(j=0; j<8; j++){
			if((pData[i]>>(7-j))&0x1){
				GPIOPinWrite(port, si, si);
			}else{
				GPIOPinWrite(port, si, ~si);
			}
			GPIOPinWrite(port, sclk, sclk);	//��������������		
			GPIOPinWrite(port, sclk, ~sclk);
		}
	}
	GPIOPinWrite(port, cs, cs);
}

/*******************************************
* �������ܣ����ݷ��ͺ��� 
* ���������port��GPIO��		   
*	        si����������
*		    so���������
* 		    cs��Ƭѡ��
* 		    sclk��ʱ������
* �����������
* �� �� ֵ����
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
* �������ܣ����ݷ��ͺ��� 
* ���������port��GPIO��		   
*	        si����������
*		    so���������
* 		    cs��Ƭѡ��
* 		    sclk��ʱ������
* �����������
* �� �� ֵ����
********************************************/
void receive(unsigned long port,
			 unsigned char si,
			 unsigned char so,
			 unsigned char cs,
			 unsigned char sclk){
	write_command(SRXON, port, si, so, cs, sclk);
}

/*******************************************
* �������ܣ�SPI Ҫ��CSnʹ��ʱʹ�� 
* ���������port��GPIO��		   
*	        si����������
*		    so���������
* 		    cs��Ƭѡ��
* 		    sclk��ʱ������
* �����������
* �� �� ֵ����
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
		if(GPIOPinRead(port, so)){		//����Ĵ���״ֵ̬
			ret |= (1<<(7-i));
		}
		GPIOPinWrite(port, sclk, ~sclk);
	}
	return ret;
}

/*******************************************
* �������ܣ�cc2420оƬʹ�� 
* ���������port��ĳƬcc2420��GPIO��		   
*	        cs��Ƭѡ
* �����������
* �� �� ֵ����
********************************************/
void cc2420_cs_enable(unsigned long port,
			 		  unsigned char cs){
	GPIOPinWrite(port, cs, ~cs);	//cs�͵�ƽʱʹ��					  
}

/*******************************************
* �������ܣ�cc2420оƬ���� 
* ���������port��ĳƬcc2420��GPIO��		   
*	        cs��Ƭѡ
* �����������
* �� �� ֵ����
********************************************/
void cc2420_cs_disable(unsigned long port,
			 		   unsigned char cs){
	GPIOPinWrite(port, cs, cs);	//cs�ߵ�ƽʱ����
}


