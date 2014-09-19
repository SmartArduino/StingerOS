#include "memory.h"

extern memory_t *memFreeList;
extern memory_t memStruct[DEF_MEM_STRUCT_NUM];
extern memory_t *pMem8, *pMem16, *pMem32;
extern unsigned char mem8[100][8], mem16[100][16], mem32[100][32];

//unsigned char pool[100][32]={0};
/*******************************************
* 函数功能：内存池初始化函数
* 输入参数：addr：内存池首地址
*			nBlks：池包含内存块数
*			blkSize：块大小
*			err：错误值
* 输出参数：无
* 返 回 值：两个字节CRC校验码
********************************************/
void memory_init(void){
	int i=0;
	unsigned char err;

	memFreeList=memStruct;
	for(i=0; i<DEF_MEM_STRUCT_NUM-1; i++){
		memStruct[i].next=&memStruct[i+1];
	}
	memStruct[DEF_MEM_STRUCT_NUM-1].next=0;

	pMem8=memory_create(mem8, 100, 8, &err);
	pMem16=memory_create(mem16, 100, 16, &err);
	pMem32=memory_create(mem32, 100, 32, &err);			   
}
/*******************************************
* 函数功能：内存池生成函数
* 输入参数：addr：内存池首地址
*			nBlks：池包含内存块数
*			blkSize：块大小
*			err：错误值
* 输出参数：无
* 返 回 值：两个字节CRC校验码
********************************************/
memory_t *memory_create(void *addr, 
						unsigned long nBlks, 
						unsigned long blkSize, 
						unsigned char *err){
	memory_t *pMem;
	//memory_t *newMemStruct;
	void **pLink;
	unsigned char *pBlk;
	int i=0;

	if(addr == 0){
		*err=INVALID_ADDRESS;
		return 0;
	}else if(nBlks < 2){
		*err=INVALID_BLOCKS;
		return 0;
	}

	pMem=memFreeList;
	if(memFreeList != 0){
		memFreeList=memFreeList->next;
	}else{
		//newMemStruct=(memory_t *)malloc(sizeof(memory_t));
		//memFreeList=newMemStruct;
	}

	pLink=(void **)addr;
	pBlk=(unsigned char *)addr+blkSize;
	for(i=0; i<(nBlks-1); i++){
		*pLink=(void *)pBlk;
		pLink=(void **)pBlk;
		pBlk += blkSize;
	}
	*pLink=0;

	pMem->memAddr=addr;
	pMem->next=addr;
	pMem->memBlkSize=blkSize;
	pMem->memBlksNum=nBlks;
	pMem->memFreeNum=nBlks;

	*err=NO_ERROR;

	return pMem;
}

/*******************************************
* 函数功能：内存池销毁函数
* 输入参数：addr：内存池首地址
*			nBlks：池包含内存块数
*			blkSize：块大小
*			err：错误值
* 输出参数：无
* 返 回 值：两个字节CRC校验码
********************************************/
void memory_destroy(void){

}

/*******************************************
* 函数功能：获取内存块
* 输入参数：pMem：内存池首地址
*			err：错误值
* 输出参数：无
* 返 回 值：无
********************************************/
void *memory_get(memory_t *pMem, unsigned char *err){
	void *pBlk;

	if(pMem == 0){
		*err=INVALID_ADDRESS;
		return 0;
	}

	if(pMem->memFreeNum > 0){
		pBlk=pMem->next;
		pMem->next=*(void **)pBlk;
		pMem->memFreeNum--;
		*err=NO_ERROR;
		return pBlk;
	}

	*err=NO_FREE_BLOCKS;
	return 0;
}

/*******************************************
* 函数功能：内存块放回
* 输入参数：pMem：内存池首地址
*			err：错误值
* 输出参数：无
* 返 回 值：无
********************************************/
unsigned char memory_put(memory_t *pMem, void *pBlk){

	if(pMem->memFreeNum >= pMem->memBlksNum){
		return MEM_FULL;
	}

	*(void **)pBlk=pMem->next;
	pMem->next=pBlk;
	pMem->memFreeNum++;

	return NO_ERROR;
}
