#ifndef	MEMORY_H
#define MEMORY_H

#define DEF_MEM_STRUCT_NUM	8
#define	INVALID_ADDRESS		255
#define INVALID_BLOCKS		254
#define NO_FREE_BLOCKS		253
#define MEM_FULL			252
#define NO_ERROR			0

typedef struct memory{
	void *memAddr;
	struct memory *next;
	unsigned long memBlkSize;		// 块大小
	unsigned long memBlksNum;		// 总块数
	unsigned long memFreeNum;		// 空闲块数
}memory_t;

void memory_init(void);
memory_t *memory_create(void *addr, 
						unsigned long nBlks, 
						unsigned long blkSize, 
						unsigned char *err);
void *memory_get(memory_t *pMem, unsigned char *err);
unsigned char memory_put(memory_t *pMem, void *pBlk);

#endif
