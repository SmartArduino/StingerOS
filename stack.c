#include "stack.h"
#include "hal.h"
#include "cc2420.h"
#include "macro.h"
#include "string.h"

/*******************************************
* �������ܣ�����CRCУ����
* ���������ptr���ַ����׵�ַ
*			len���ַ�������
* �����������
* �� �� ֵ�������ֽ�CRCУ����
********************************************/
unsigned short crc_calculate(unsigned char *ptr, unsigned char len){
	unsigned short crc=0;
	unsigned char i=0;
/*	unsigned char dataH=0;
	unsigned short table[256]={		// CRC��ʽ������ʽΪ0x11021
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
		0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
		0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
		0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
		0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
		0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
		0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
		0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
		0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
		0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
		0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
		0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
		0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
		0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
		0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
		0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
		0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
		0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
		0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
		0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
		0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
	};
	
	while(len-- != 0){
		dataH = (unsigned char)(crc/256);
		crc <<= 8;
		crc ^= table[dataH^*ptr];
		ptr++;
	}	  */

	while(len-- != 0){
		for(i=0x80; i!=0; i/=2){
			if((crc&0x8000) != 0){
				crc *= 2;
				crc ^= 0x1021;
			}else{
				crc *= 2;
			}

			if((*ptr&i) != 0){
				crc ^= 0x1021;
			}	
		}
		ptr++;
	}

	return crc;
}

/*******************************************
* �������ܣ�У������
* ���������ptr���ַ����׵�ַ
*			len���ַ�������
* �����������
* �� �� ֵ�������ִ��Ƿ����������0����֮Ϊ1
********************************************/
unsigned char crc_check(unsigned char *ptr, unsigned char len){
	unsigned short crcOriginal=0, crc=0;

	crcOriginal |= (ptr[len-2]<<8);
	crcOriginal |= ptr[len-1];

	crc=crc_calculate(ptr, len-2);
	if(crc != crcOriginal){
		return 0;
	}

	return 1;
}

/*******************************************
* �������ܣ�CRCУ�������ɺ���
* ���������ptr���ַ����׵�ַ
*			len���ַ�������
* �����������
* �� �� ֵ����
********************************************/
void crc_generate(unsigned char *ptr, unsigned char len){
	unsigned short crc=0;

	crc=crc_calculate(ptr, len-2);
	ptr[len-2]=(unsigned char)(crc>>8);
	ptr[len-1]=(unsigned char)(crc&0x00ff);
}

/*******************************************
* �������ܣ�ZigBee���ݰ����ɺ���
* ���������ptr���ַ����׵�ַ
*			len���ַ�������
* �����������
* �� �� ֵ����
********************************************/
zb_packet_t *zb_packet_create(unsigned char *err){
	zb_packet_t *pPacket;
	unsigned char errNum;
	
	pPacket=(zb_packet_t *)os_mem_alloc(sizeof(zb_packet_t));
	*err=errNum;

	return pPacket;	
}

/*******************************************
* �������ܣ����ݰ��ͷ�
* ���������packet�����ݰ��ṹָ��
* �����������
* �� �� ֵ����
********************************************/
unsigned char zb_packet_release(zb_packet_t *packet){

	return 1;
}

/*******************************************
* �������ܣ������ͷ�
* ���������buf�����ݰ��ṹָ��
* �����������
* �� �� ֵ����
********************************************/
unsigned char zb_buffer_release(unsigned char *buf){

	return 1;
}

/*******************************************
* �������ܣ���ȡ������
* ���������packet�����ݰ��ṹָ��
* ���������err 0��ʾ�ɹ���1��ʾʧ��
* �� �� ֵ����
********************************************/
unsigned char *zb_buffer_get(unsigned char size, unsigned char *err){
	unsigned char *buf;

	buf=(unsigned char *)os_mem_alloc(size);

	*err=0;

	return buf;
}

/*******************************************
* �������ܣ�����֡������FCF
* ���������packet�����ݰ��ṹָ��
*			fcf������
* �����������
* �� �� ֵ����
********************************************/
void zb_set_fcf(zb_packet_t *packet, unsigned char *fcf){
	(packet->fcf)[0]=*fcf++;
	(packet->fcf)[1]=*fcf;
}

/*******************************************
* �������ܣ�����ԴPAN��ʶ��
* ���������packet�����ݰ��ṹָ��
*			id������
* �����������
* �� �� ֵ����
********************************************/
void zb_set_source_pan_id(zb_packet_t *packet, unsigned char *id){
	(packet->srcPanID)[0]=*id++;
	(packet->srcPanID)[1]=*id;
}

/*******************************************
* �������ܣ�����ԴPAN��ʶ��
* ���������packet�����ݰ��ṹָ��
*			id������
* �����������
* �� �� ֵ����
********************************************/
void zb_set_source_id(zb_packet_t *packet, unsigned char *id){
	(packet->srcID)[0]=*id++;
	(packet->srcID)[1]=*id;
}

/*******************************************
* �������ܣ����ݰ�д�뻺��
* ���������packet�����ݰ��ṹָ��
*			buf������
* �����������
* �� �� ֵ����
********************************************/
void zb_packet_to_cache(zb_packet_t *packet, unsigned char *buf){
	unsigned char *temp;

	temp=buf;
	//�����ݰ��е����ݰ�֡��ʽ��д������
	*buf=packet->num+1;
	*(++buf)=(packet->fcf)[1];
	*(++buf)=(packet->fcf)[0];
	*(++buf)=packet->seq;
	*(++buf)=(packet->desPanID)[1];
	*(++buf)=(packet->desPanID)[0];
	*(++buf)=(packet->desID)[1];
	*(++buf)=(packet->desID)[0];
	*(++buf)=(packet->srcPanID)[1];
	*(++buf)=(packet->srcPanID)[0];
	*(++buf)=(packet->srcID)[1];
	*(++buf)=(packet->srcID)[0];
	*(++buf)=packet->type;
	*(++buf)=packet->sizeOfPayload;

	//д��payload
	++buf;
	string_copy((char *)buf, (char *)(packet->payload), packet->sizeOfPayload);

	crc_generate(temp, packet->num);
}

/*******************************************
* �������ܣ��ӻ�������ȡ������
* ���������packet�����ݰ��ṹָ��
*			buf������
* ���������packet�����ݰ��ṹָ��
* �� �� ֵ��CRCУ������1��ʾУ��ͨ������֮Ϊ0
********************************************/
unsigned char zb_cache_to_packet(zb_packet_t *packet, unsigned char *buf){
	
	if(!crc_check(buf, *buf-1)){	//�����е�һ���ֽ�Ϊ������������
		return 0;
	}

	packet->num=*buf-1;
	buf++;
	(packet->fcf)[1]=*buf++;
	(packet->fcf)[0]=*buf++;
	packet->seq=*buf++;
	(packet->desPanID)[1]=*buf++;
	(packet->desPanID)[0]=*buf++;
	(packet->desID)[1]=*buf++;
	(packet->desID)[0]=*buf++;
	(packet->srcPanID)[1]=*buf++;
	(packet->srcPanID)[0]=*buf++;
	(packet->srcID)[1]=*buf++;
	(packet->srcID)[0]=*buf++;
	packet->type=*buf++;
	packet->sizeOfPayload=*buf++;
	packet->payload=buf;

	return 1;
}
										        
/*******************************************
* �������ܣ�ZigBee���ݷ��ͺ���
* ���������packet�����ݰ�
*			chipNum������ģ��
* �����������
* �� �� ֵ����
********************************************/
unsigned char zb_packet_send(zb_packet_t *packet, unsigned char *buf, unsigned char chipNum){
	//unsigned char err=0;

	if(packet == 0){
		return 0;
	}

	zb_packet_to_cache(packet, buf);

	switch(chipNum){
		case 0:
			write_txfifo(packet->num, buf, CHIP_1_PORT, CHIP_1_SI, CHIP_1_SO, CHIP_1_CS, CHIP_1_SCLK);
			send(CHIP_1_PORT, CHIP_1_SI, CHIP_1_SO, CHIP_1_CS, CHIP_1_SCLK);
			break;
		case 1:
			write_txfifo(packet->num, buf, CHIP_2_PORT, CHIP_2_SI, CHIP_2_SO, CHIP_2_CS, CHIP_2_SCLK);
			send(CHIP_2_PORT, CHIP_2_SI, CHIP_2_SO, CHIP_2_CS, CHIP_2_SCLK);
			break;
		case 2:
			write_txfifo(packet->num, buf, CHIP_3_PORT, CHIP_3_SI, CHIP_3_SO, CHIP_3_CS, CHIP_3_SCLK);
			send(CHIP_3_PORT, CHIP_3_SI, CHIP_3_SO, CHIP_3_CS, CHIP_3_SCLK);
			break;
		case 3:
			write_txfifo(packet->num, buf, CHIP_4_PORT, CHIP_4_SI, CHIP_4_SO, CHIP_4_CS, CHIP_4_SCLK);
			send(CHIP_4_PORT, CHIP_4_SI, CHIP_4_SO, CHIP_4_CS, CHIP_4_SCLK);
			break;
		case 4:
			write_txfifo(packet->num, buf, CHIP_5_PORT, CHIP_5_SI, CHIP_5_SO, CHIP_5_CS, CHIP_5_SCLK);
			send(CHIP_5_PORT, CHIP_5_SI, CHIP_5_SO, CHIP_5_CS, CHIP_5_SCLK);
			break;
		default:
			//UARTprintf("Error in random!\n");
			break;
	}

	return 1;
}

/*******************************************
* �������ܣ�ZigBee���ݷ��ͺ���
* ���������packet�����ݰ�
*			chipNum������ģ��
* �����������
* �� �� ֵ����
********************************************/
unsigned char zb_packet_receive(zb_packet_t *packet, unsigned char *buf){
	unsigned char ret=0;

	read_rxfifo(packet->num, buf, GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);
	write_command(SFLUSHRX, GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);
	write_command(SFLUSHRX, GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);	//�������Σ����ĵ�32ҳ��ͷ

	ret=zb_cache_to_packet(packet, buf);
	if(!ret){
		return 0;
	}

	return 1;
}

/*******************************************
* �������ܣ���Ƶģ���ʼ��
* �����������
* �����������
* �� �� ֵ����
********************************************/
void zb_rf_configuration(void){
	//���õ�һƬcc2420
	cc2420_configuration(GPIO_PORTG_BASE, GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_6);
	powerdown_to_idle_mode(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);

	//���õڶ�Ƭcc2420
	cc2420_configuration(GPIO_PORTG_BASE, GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_2);
	powerdown_to_idle_mode(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_4);

	//���õ���Ƭcc2420
	cc2420_configuration(GPIO_PORTE_BASE, GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_4);
	powerdown_to_idle_mode(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1, GPIO_PIN_0);

	//���õ���Ƭcc2420
	cc2420_configuration(GPIO_PORTB_BASE, GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_6);
	powerdown_to_idle_mode(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1, GPIO_PIN_0);

	//���õ���Ƭcc2420
	cc2420_configuration(GPIO_PORTD_BASE, GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_7);
	powerdown_to_idle_mode(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1, GPIO_PIN_0);	
}

/*******************************************
* �������ܣ���Ƶģ���ʼ��
* �����������
* �����������
* �� �� ֵ����
********************************************/
void zb_receive_configuration(void){
	//���ڽ��նˣ�ֻ���õ�һƬcc2420
	cc2420_configuration(GPIO_PORTG_BASE, GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_6);
	powerdown_to_idle_mode(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2);
}

/*******************************************
* �������ܣ�cc2420���뷢��ǰ����״̬
* �����������
* �����������
* �� �� ֵ����
********************************************/
void zb_cc2420_status_idle(void){
	//5��cc2420�������״̬
	all_status_idle(CHIP_1_PORT, CHIP_1_SI, CHIP_1_SO, CHIP_1_CS, CHIP_1_SCLK);
	all_status_idle(CHIP_2_PORT, CHIP_2_SI, CHIP_2_SO, CHIP_2_CS, CHIP_2_SCLK);
	all_status_idle(CHIP_3_PORT, CHIP_3_SI, CHIP_3_SO, CHIP_3_CS, CHIP_3_SCLK);
	all_status_idle(CHIP_4_PORT, CHIP_4_SI, CHIP_4_SO, CHIP_4_CS, CHIP_4_SCLK);
	all_status_idle(CHIP_5_PORT, CHIP_5_SI, CHIP_5_SO, CHIP_5_CS, CHIP_5_SCLK);
}

