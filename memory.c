#include "memory.h"

extern memory_t *memFreeList;
extern memory_t memStruct[DEF_MEM_STRUCT_NUM];
extern memory_t *pMem8, *pMem16, *pMem32;
extern unsigned char mem8[100][8], mem16[100][16], mem32[100][32];

//unsigned char pool[100][32]={0};
/*******************************************
* �������ܣ��ڴ�س�ʼ������
* ���������addr���ڴ���׵�ַ
*			nBlks���ذ����ڴ����
*			blkSize�����С
*			err������ֵ
* �����������
* �� �� ֵ�������ֽ�CRCУ����
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
* �������ܣ��ڴ�����ɺ���
* ���������addr���ڴ���׵�ַ
*			nBlks���ذ����ڴ����
*			blkSize�����С
*			err������ֵ
* �����������
* �� �� ֵ�������ֽ�CRCУ����
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
* �������ܣ��ڴ�����ٺ���
* ���������addr���ڴ���׵�ַ
*			nBlks���ذ����ڴ����
*			blkSize�����С
*			err������ֵ
* �����������
* �� �� ֵ�������ֽ�CRCУ����
********************************************/
void memory_destroy(void){

}

/*******************************************
* �������ܣ���ȡ�ڴ��
* ���������pMem���ڴ���׵�ַ
*			err������ֵ
* �����������
* �� �� ֵ����
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
* �������ܣ��ڴ��Ż�
* ���������pMem���ڴ���׵�ַ
*			err������ֵ
* �����������
* �� �� ֵ����
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
