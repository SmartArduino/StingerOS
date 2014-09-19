#ifndef _STACK_H_
#define _STACK_H_

#define COMMAND_REQUEST		100
#define COMMAND_ANSWER		101
#define COMMAND_CONNECT		102

typedef struct zb_packet{
	unsigned char num;			// 总共传输数据的总数
	unsigned char fcf[2];		// 数据帧控制域(frame control field)
	unsigned char seq;			// 数据帧序列号(data sequence number)
	unsigned char desPanID[2];	// 目的PAN标识符
	unsigned char desID[2];		// 目的地址
	unsigned char srcPanID[2];	// 源PAN标识符
	unsigned char srcID[2];		// 源地址
	unsigned char type;			// 数据帧类型
	unsigned char sizeOfPayload;// payload长度
	unsigned char *payload;		// 数据
}zb_packet_t;

unsigned short crc_calculate(unsigned char *ptr, unsigned char len);
unsigned char crc_check(unsigned char *ptr, unsigned char len);
void crc_generate(unsigned char *ptr, unsigned char len);
zb_packet_t *zb_packet_create(unsigned char *err);
void zb_set_fcf(zb_packet_t *packet, unsigned char *fcf);
void zb_set_source_pan_id(zb_packet_t *packet, unsigned char *id);
void zb_set_source_id(zb_packet_t *packet, unsigned char *id);
unsigned char zb_packet_send(zb_packet_t *packet, unsigned char *buf, unsigned char chipNum);
unsigned char zb_packet_receive(zb_packet_t *packet, unsigned char *buf);
unsigned char zb_packet_release(zb_packet_t *packet);
unsigned char zb_buffer_release(unsigned char *buf);
unsigned char *zb_buffer_get(unsigned char size, unsigned char *err);
void zb_rf_configuration(void);
void zb_receive_configuration(void);
void zb_cc2420_status_idle(void);

#endif

