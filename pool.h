#ifndef _POOL_H_
#define _POOL_H_

/*******************************************
* Macro definition
********************************************/
#define MAX_BUDDY_ORDER     9
#define BLOCK_SIZE          32
#define POOL_DEFAULT_SIZE   10240
#define ORDER_MAX_BUDDY     (1<<(MAX_BUDDY_ORDER-1))

#define BUDDY_END(addr, order)          (addr+(1<<order)-1)
#define NEXT_BUDDY_START(addr, order)   (addr+(1<<order))
#define PREV_BUDDY_START(addr, order)   (addr-(1<<order))

/*******************************************
* Struct definition
********************************************/
typedef struct buddy{
    unsigned char *vAddr;
    unsigned int used;
    int order;
    int bf;
    struct buddy *pPrev;
    struct buddy *pNext;
    struct buddy *pLeftChild;
    struct buddy *pRightChild;
}buddy_t;

/*******************************************
* Function declaration
********************************************/
int pool_create(unsigned int poolSize);
int is_pool_empty(void);
void *pool_mem_alloc(int size);
void pool_mem_free(void *addr);
void pool_destroy(void);

#endif
