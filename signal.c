#include <stdio.h>
#include <stdarg.h>
#include "signal.h"
#include "pool.h"
#include "slot.h"
#include "string.h"

port_t gHashTable[PRIME];

/*******************************************
* asm function:
* Input:function address, stack
* Output:none
********************************************/
/*void call_func(void* pFuncAddr, int *pStack){
    int ret;
    __asm__(
        "PUSHL %%ESI\n\t"
        "MOVL %%ESP,%%ESI\n\t"
        "MOVL %2,%%ESP\n\t"
        "CALL %1\n\t"
        "MOVL %%ESI,%%ESP\n\t"
        "POPL %%ESI"
        :"=g"(ret)
        :"g"(pFuncAddr), "g"(pStack)

    );
}*/

/*******************************************
* asm test function:
* Input:
* Output:
********************************************/
/*int test_asm(int x, int y){
    int ret;
    __asm__(
        "PUSHL %%EAX\n\t"
        "MOVL %1, %%EAX\n\t"
        "ADDL %2, %%EAX\n\t"
        "MOVL %%EAX, %0\n\t"
        "POPL %%EAX"
        :"=g"(ret)
        :"g"(x), "g"(y)
        :"ax", "memory"
    );
    return ret;
}*/

/*******************************************
* Hash table initialization:
* Input:none
* Output:none
********************************************/
void hash_table_init(void){
    int i, j;

    for(i=0; i<PRIME; i++){
        gHashTable[i].pNext=NULL;
        for(j=0; j<SIGNAL_NAME_LENGHTH; j++){
            (gHashTable[i].sig)[j]='\0';
        }
    }
}

/*******************************************
* Initialize signal and slot:
* Input:none
* Output:none
********************************************/
void signal_slot_init(void){
    hash_table_init();
    //port_array_init();
}

/*******************************************
* function:
* Input:
* Output:
********************************************/
void hash_add_list_tail(port_t *pHead, port_t *pNewElem){

}

/*******************************************
* Insert new element to hash list:
* Input:previous element, new element
* Output:none
********************************************/
void hash_list_insert(port_t *pHead, port_t *pNewElem){
    port_t *pTmp;

    pTmp=pHead->pNext;
    pHead->pNext=pNewElem;
    pNewElem->pNext=pTmp;
}

/*******************************************
* Remove element from hash list:
* Input:base, exponent
* Output:value of power
********************************************/
void hash_list_remove(port_t *pPrev, port_t *pTar){
    pPrev->pNext=pTar->pNext;
}

/*******************************************
* Get value from hash:
* Input:key
* Output:index of hash list
********************************************/
int hash_get(char *pKey){
    int value=0;
    char *pTmp;

    if(*pKey == '\0'){
        printf("Invaliad signal.(in hash_get)\n");
        return -1;
    }
    pTmp=pKey;
    while(*pTmp != '\0'){
        value+=*pTmp;
        pTmp++;
    }
    value%=PRIME;
    return value;
}

/*******************************************
* Connect signal and slot:
* Input:signal, slot
* Output:statue of connect
********************************************/
STATUS connect(char *pSig, void *pSlot){
    port_t *pPort;
    int val, len;

    if(*pSig == '\0'){
        printf("Invalid signal.(in connect)\n");
        return 0;
    }else if(pSlot == NULL){
        printf("Invalid slot.(in connect)\n");
        return 0;
    }
    pPort=(port_t *)pool_mem_alloc(sizeof(port_t));
    if(pPort == NULL){
        printf("Alloc memory for port struct failed.\n");
        return 0;
    }
    val=hash_get(pSig);
    if(val == -1){
        return 0;
    }
    len=string_length(pSig);
    string_copy(pPort->sig, pSig, len);
    pPort->pSlot=pSlot;
    pPort->pNext=NULL;
    hash_list_insert(&gHashTable[val], pPort);
    return 1;
}

/*******************************************
* Disconnect signal and slot:
* Input:signal
* Output:status of disconnect
********************************************/
STATUS disconnect(char *pSig){
    int val;
    port_t *pTmp=NULL, *pPrev;

    if(*pSig == '\0'){
        printf("Invalid signal.(in connect)\n");
        return 0;
    }
    val=hash_get(pSig);
    if(val == -1){
        return 0;
    }
    if(gHashTable[val].pNext == NULL){
        return 0;
    }
    pPrev=&gHashTable[val];
    pTmp=gHashTable[val].pNext;
    while(1){
        if(pTmp == NULL){
            break;
        }
        if(string_compare(pSig, pTmp->sig) == 0){
            hash_list_remove(pPrev, pTmp);
            pool_mem_free(pTmp);
            pTmp=pPrev->pNext;
        }else{
            pTmp=pTmp->pNext;
            pPrev=pPrev->pNext;
        }
    }
    return 1;
}

/*******************************************
* Arguments processing:
* Input:number of arguments, stack, point to struct port
* Output:none
********************************************/
void arg_process(int argNum, int *pArgList, port_t *pPort){
    switch(argNum){
        case 0:
        case 1:
            (*(pPort->pSlot))(*pArgList);
            break;
        case 2:
            (*(pPort->pSlot))(*pArgList, *(pArgList+1));
            break;
        case 3:
            (*(pPort->pSlot))(*pArgList, *(pArgList+1), *(pArgList+2));
            break;
        case 4:
            (*(pPort->pSlot))(*pArgList, *(pArgList+1), *(pArgList+2), *(pArgList+3));
            break;
        case 5:
            (*(pPort->pSlot))(*pArgList, *(pArgList+1), *(pArgList+2), *(pArgList+3), *(pArgList+4));
            break;
        case 6:
            (*(pPort->pSlot))(*pArgList, *(pArgList+1), *(pArgList+2), *(pArgList+3), *(pArgList+4), *(pArgList+5));
            break;
        case 7:
            (*(pPort->pSlot))(*pArgList, *(pArgList+1), *(pArgList+2), *(pArgList+3), *(pArgList+4), *(pArgList+5), *(pArgList+6));
            break;
        case 8:
            (*(pPort->pSlot))(*pArgList, *(pArgList+1), *(pArgList+2), *(pArgList+3), *(pArgList+4), *(pArgList+5), *(pArgList+6), *(pArgList+7));
            break;
        case 9:
            (*(pPort->pSlot))(*pArgList, *(pArgList+1), *(pArgList+2), *(pArgList+3), *(pArgList+4), *(pArgList+5), *(pArgList+6), *(pArgList+7), *(pArgList+8));
            break;
        default:
            break;
    }

}

/*******************************************
* Emit signal:
* Input:signal, number of arguments
* Output:none
********************************************/
void emit(char *pSig, int argNum, ...){
    int count=argNum, key;
    int *pArg=NULL, *pTmp=NULL;
    port_t *pPort;
    va_list ap;
    va_start(ap, argNum);

    pArg=(int *)pool_mem_alloc((count+1)*4);
    if(pArg == NULL){
        printf("Alloc memory from pool failed.\n");
        return;
    }
    pTmp=pArg;
    count=argNum;
    while(count-- != 0){
        *pTmp=va_arg(ap, int);
        pTmp++;
    }

    key=hash_get(pSig);
    if(key == -1){
        return;
    }
    pPort=gHashTable[key].pNext;
    while(1){
        if(pPort == NULL){
            break;
        }
        if(string_compare(pSig, pPort->sig) == 0){
            //printf("Find slot 0x%08x.\n", (unsigned int)(pPort->pSlot));
            //call_func(pPort->pSlot, pArg);
            arg_process(argNum, pArg, pPort);
            pPort=pPort->pNext;
        }else{
            pPort=pPort->pNext;
        }
    }

    if(pArg != NULL){
        pool_mem_free(pArg);
    }
}
