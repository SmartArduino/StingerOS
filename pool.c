#include <stdio.h>
#include <stdlib.h>
#include "pool.h"
#include "tree.h"

/*******************************************
* Global var definition
********************************************/
buddy_t gBuddyHead[MAX_BUDDY_ORDER];
buddy_t *pBuddyStart=NULL;
unsigned char *pBaseAddr=NULL;
buddy_t *pAVLTree=NULL;

/*******************************************
* Power function:
* Input:base, exponent
* Output:value of power
********************************************/
int power(int base, int exp){
    int ret, i;

    ret=1;
    for(i=0; i<exp; i++){
        ret*=base;
    }
    return ret;
}

/*******************************************
* Pointer of buddy head
* Input:pointer of buddy list head
* Output:none
********************************************/
void buddy_head_init(buddy_t *pHead){
    pHead->pPrev=pHead;
    pHead->pNext=pHead;
}

/*******************************************
* Initialization of buddy list
* Input:
* Output:none
********************************************/
void buddy_list_init(buddy_t **pList){
    (*pList)->pPrev=*pList;
    (*pList)->pNext=*pList;
}

/*******************************************
* Add new buddy to list tail
* Input:buddy list head, new buddy
* Output:none
********************************************/
void buddy_add_list_tail(buddy_t *pHead, buddy_t **pNewElem){
    (*pNewElem)->pNext=pHead;
    (*pNewElem)->pPrev=pHead->pPrev;
    pHead->pPrev->pNext=*pNewElem;
    pHead->pPrev=*pNewElem;
}

/*******************************************
* Remove a chain from a buddy list
* Input:head of a chain, tail of the chain
* Output:none
********************************************/
void buddy_list_remove_chain(buddy_t *pHead, buddy_t *pTail){
    pHead->pPrev->pNext=pTail->pNext;
    pTail->pNext->pPrev=pHead->pPrev;
}

/*******************************************
* Add a chain to a buddy list
* Input:head of a chain, tail of the chain, new chain
* Output:none
********************************************/
void buddy_list_add_chain(buddy_t *pHead, buddy_t *pTail, buddy_t *pList){
    pHead->pPrev=pList;
    pTail->pNext=pList->pNext;
    pList->pNext->pPrev=pTail;
    pList->pNext=pHead;
}

/*******************************************
* Check a buddy list
* Input:
* Output:
********************************************/
int buddy_is_list_empty(buddy_t *pHead){
    if(pHead->pNext == pHead){
        return 1;
    }else{
        return 0;
    }
}

/*******************************************
* Initialization of buddy list head
* Input:none
* Output:none
********************************************/
void init_head_for_each(void){
    int i;
    for(i=0; i<MAX_BUDDY_ORDER; i++){
        gBuddyHead[i].order=-1;
        gBuddyHead[i].used=1;
        buddy_head_init(&gBuddyHead[i]);
    }
}

/*******************************************
* Get end address of a pool
* Input:size of pool
* Output:end address of pool
********************************************/
int get_pool_end(unsigned int size){
    return (size/(sizeof(buddy_t)+BLOCK_SIZE)*BLOCK_SIZE);
}

/*******************************************
* Get numbers of memory block in a pool
* Input:size of memory in pool
* Output:numbers of memory block
********************************************/
int get_block_num(unsigned int size){
    return (size/(sizeof(buddy_t)+BLOCK_SIZE));
}

/*******************************************
* Get the max order that pool could alloc
* Input:size of memory in pool
* Output:order
********************************************/
int max_order_buddy_could_alloc(unsigned int size){
    int tmp, i;

    if(size == 0){
        return -1;
    }
    tmp=size/(sizeof(buddy_t)+BLOCK_SIZE);
    for(i=MAX_BUDDY_ORDER-1; i>0; i--){
        if(tmp/(1<<i)){
            return i;
        }
    }
    if(i == 0){
        return -1;
    }
    return i;
}

/*******************************************
* Initialization of a pool
* Input:start address of memory, size of memory
* Output:none
********************************************/
void pool_init(unsigned char *pStartAddr, unsigned int size){
    int blockNum=0, i, tmp, taller=0;
    unsigned int allocMaxOrder;
    buddy_t *pBuddy=NULL;

    if(pStartAddr == NULL){
        printf("Invalid pool start address.(in pool_init)\n");
        return;
    }else if(size == 0){
        printf("Invalid pool size.(in pool_init)\n");
        return;
    }
    tmp=max_order_buddy_could_alloc(size);
    if(tmp == -1){
        printf("Invalid order.(in pool_init)\n");
        return;
    }
    allocMaxOrder=(unsigned int)tmp;
    pBaseAddr=pStartAddr;
    init_head_for_each();
    blockNum=get_block_num(size);
    pBuddyStart=(buddy_t *)(pStartAddr+get_pool_end(size));
    pBuddy=pBuddyStart;
    for(i=0; i<blockNum; i++,pBuddy++){
        pBuddy->vAddr=pStartAddr+i*BLOCK_SIZE;
        pBuddy->used=0;
        buddy_list_init(&pBuddy);
        if(i < (blockNum/(1<<allocMaxOrder))*(1<<allocMaxOrder)){
            if((i&(~(1<<allocMaxOrder)&0xffffffff>>(32-allocMaxOrder))) == 0){
                pBuddy->order=allocMaxOrder;
            }else{
                pBuddy->order=-1;
            }
            buddy_add_list_tail(&gBuddyHead[allocMaxOrder], &pBuddy);
        }else{
            pBuddy->order=0;
            buddy_add_list_tail(&gBuddyHead[0], &pBuddy);
        }
        pBuddy->bf=EQU_HIGH;
        insert_tree(&pAVLTree, pBuddy, &taller);
    }
}

/*******************************************
* List pool information
* Input:none
* Output:none
********************************************/
void pool_info_list(unsigned int size){
    int i=0;

    printf("###########################################\n");
    printf("#             pool information             \n");
    printf("# pool size: %d\n", size);
    printf("# number of memory block: %d\n", get_block_num(size));
    printf("# start of pool: 0x%08x\n", (unsigned int)pBaseAddr);
    printf("# start address of buddy struct: 0x%08x\n", (unsigned int)pBuddyStart);
    printf("# memory that can alloc in pool is: %dbytes\n", get_block_num(size)*BLOCK_SIZE);
    for(i=0; i<MAX_BUDDY_ORDER; i++){
        if(buddy_is_list_empty(&gBuddyHead[i])){
            printf("# buddy list %d is empty\n", i);
        }else{
            printf("# buddy list %d is not empty\n", i);
        }
    }
    printf("###########################################\n");
}

/*******************************************
* Create pool
* Input:size of pool
* Output:whether success
********************************************/
int pool_create(unsigned int poolSize){
    void *pPool=NULL;

    if(poolSize == 0){
        return 0;
    }
    pPool=malloc(poolSize);
    if(pPool == NULL){
        return 0;
    }
    pool_init(pPool, poolSize);
    //pool_info_list(poolSize);
    return 1;
}

/*******************************************
* Check a pool
* Input:none
* Output:whether a pool is empty
********************************************/
int is_pool_empty(void){
    int i;

    for(i=0; i<MAX_BUDDY_ORDER; i++){
        if(gBuddyHead[i].pNext != &gBuddyHead[i]){
            return 0;
        }
    }
    return 1;
}

/*******************************************
* Get buddy struct from buddy list
* Input:order
* Output:pointer of buddy struct
********************************************/
buddy_t *get_buddy_from_list(int order){
    buddy_t *pBud=NULL, *pBuddyEnd;
    int newOrder;

    if(order>MAX_BUDDY_ORDER-1 || order<0){
        printf("Invalid order.(in get_buddy_from_list)\n");
        return NULL;
    }
    newOrder=order;
    for(; newOrder<MAX_BUDDY_ORDER; newOrder++){
        if(buddy_is_list_empty(&gBuddyHead[newOrder])){
            continue;
        }else{
            pBud=gBuddyHead[newOrder].pNext;
            pBuddyEnd=BUDDY_END(pBud, newOrder);
            pBuddyEnd->pNext->pPrev=&gBuddyHead[newOrder];
            gBuddyHead[newOrder].pNext=pBuddyEnd->pNext;
            goto GET_BUDDY_OK;
        }
    }
    return NULL;

GET_BUDDY_OK:
    for(newOrder--; newOrder>=order; newOrder--){
        pBud->order=newOrder;
        pBuddyEnd=BUDDY_END(pBud, newOrder);
        buddy_list_add_chain(pBud, pBuddyEnd, &gBuddyHead[newOrder]);
        pBud=NEXT_BUDDY_START(pBud, newOrder);
    }
    pBud->used=1;
    pBud->order=order;
    return pBud;
}

/*******************************************
* Get order according to the size of memory
* Input:size of memory
* Output:order
********************************************/
int get_order(int size){
    int order, i;

    if(size <= 0){
        printf("Invalid memory size.(in get_order)\n");
    }
    for(i=MAX_BUDDY_ORDER-1; i>0; --i){
        if(size/(power(2,i)*BLOCK_SIZE) == 0){
            if(size == power(2,i-1)*BLOCK_SIZE){
                order=i-1;
                break;
            }else if(size/(power(2,i-1)*BLOCK_SIZE)!=0){
                order=i;
                break;
            }
        }else{
            if(i == 8){
                return 9999;
            }
        }
    }
    if(i == 0){
        order=0;
    }
    return order;
}

/*******************************************
* Alloc memory from pool
* Input:size of memory that wanna alloc
* Output:none
********************************************/
void *pool_mem_alloc(int size){
    int order;
    buddy_t *pBud=NULL;

    if(size == 0){
        return NULL;
    }
    order=get_order(size);
    if(order > MAX_BUDDY_ORDER-1){
        printf("User require memory more than %dbytes.(in pool_mem_alloc)\n", BLOCK_SIZE*ORDER_MAX_BUDDY);
        return NULL;
    }
    pBud=get_buddy_from_list(order);
    if(!pBud){
        return NULL;
    }

    return (pBud->vAddr);
}

/*******************************************
* Find buddy that belong to the input memory block
* Input:memory address
* Output:pointer of buddy struct
********************************************/
buddy_t *find_buddy(unsigned char *addr){
    int i, blkNum;

    if(addr == NULL){
        printf("Invalid memory address.(in find_buddy)\n");
        return NULL;
    }
    blkNum=get_block_num(POOL_DEFAULT_SIZE);
    for(i=0; i<blkNum; i++){
        if(addr == pBaseAddr+i*BLOCK_SIZE){
            return (buddy_t *)(pBuddyStart+i);
        }
    }
    return NULL;
}

/*******************************************
* Find buddy from the AVL tree according to
* the physics addr
********************************************/
buddy_t *find_buddy_from_tree(buddy_t *pTree, unsigned char *pAddr){

    if(pTree == NULL){
        printf("Target not found in the AVL tree.(in find_buddy_from_tree)\n");
		return NULL;
    }
    if(pTree->vAddr == pAddr){
        return pTree;
    }else if(pAddr < pTree->vAddr){
        return find_buddy_from_tree(pTree->pLeftChild, pAddr);
    }else{
        return find_buddy_from_tree(pTree->pRightChild, pAddr);
    }
    
}

/*******************************************
* Free memory that alloc from pool before
* Input:memory address
* Output:none
********************************************/
void pool_mem_free(void *addr){
    buddy_t *pBud, *pNextBuddy, *pPrevBuddy, *pTmp;
    int order;

    if(!addr){
        printf("Invalid pool memory address.(in pool_mem_free)\n");
        return;
    }
    //pBud=find_buddy((unsigned char *)addr);
    pBud=find_buddy_from_tree(pAVLTree, (unsigned char *)addr);
    if(!pBud){
        printf("Free buddy failed.(in pool_mem_free)\n");
        return;
    }

    if(pBud->used == 0){
        printf("Try to free unused buddy.(in pool_mem_free)\n");
        return;
    }

    order=pBud->order;
    pBud->used=0;
    for(; order<MAX_BUDDY_ORDER; order++){
        pNextBuddy=NEXT_BUDDY_START(pBud, order);
        pPrevBuddy=PREV_BUDDY_START(pBud, order);
        if((pNextBuddy->used==0) && (pNextBuddy->order==order)){
            pBud->order+=1;
            pNextBuddy->order=-1;
            buddy_list_remove_chain(pNextBuddy, BUDDY_END(pNextBuddy, order));
            pTmp=BUDDY_END(pBud, order);
            pTmp->pNext=pNextBuddy;
            pNextBuddy->pPrev=pTmp;
            continue;
        }else if((pPrevBuddy->used==0) && (pPrevBuddy->order==order)){
            pBud->order=-1;
            pTmp=BUDDY_END(pPrevBuddy, order);
            buddy_list_remove_chain(pPrevBuddy, pTmp);
            pTmp->pNext=pBud;
            pBud->pPrev=pTmp;
            pBud=pPrevBuddy;
            pBud->order+=1;
            continue;
        }else{
            break;
        }
    }
    buddy_list_add_chain(pBud, (pNextBuddy-1), &gBuddyHead[order]);
}

/*******************************************
* Destroy pool
* Input:none
* Output:none
********************************************/
void pool_destroy(void){
    int i;

    if(pBaseAddr == NULL){
        printf("Pool destroy failed.(in pool_destroy)\n");
        return;
    }
    free(pBaseAddr);
    for(i=0; i<MAX_BUDDY_ORDER; i++){
        gBuddyHead[i].pNext=&gBuddyHead[i];
        gBuddyHead[i].pPrev=&gBuddyHead[i];
        gBuddyHead[i].vAddr=NULL;
    }
    pBuddyStart=NULL;
    pBaseAddr=NULL;
}

