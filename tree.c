#include <stdio.h>
#include "tree.h"

/*******************************************
* Rotation of right tree
********************************************/
void right_rotate(MY_STRUCT **pRoot){
    MY_STRUCT *pLChild;

    pLChild=(*pRoot)->pLeftChild;
    (*pRoot)->pLeftChild=pLChild->pRightChild;
    pLChild->pRightChild=*pRoot;
    *pRoot=pLChild;
}

/*******************************************
* Rotation of left tree
********************************************/
void left_rotate(MY_STRUCT **pRoot){
    MY_STRUCT *pRChild;

    pRChild=(*pRoot)->pRightChild;
    (*pRoot)->pRightChild=pRChild->pLeftChild;
    pRChild->pLeftChild=*pRoot;
    *pRoot=pRChild;
}

/*******************************************
* Make left tree balance
********************************************/
void left_balance(MY_STRUCT **pTree){
    MY_STRUCT *pLChild;
    MY_STRUCT *pLRChild;

    pLChild=(*pTree)->pLeftChild;
    switch(pLChild->bf){
        case LEFT_HIGH:
            (*pTree)->bf=EQU_HIGH;
            pLChild->bf=EQU_HIGH;
            right_rotate(pTree);
            break;
        case RIGHT_HIGH:
            pLRChild=pLChild->pRightChild;
            switch(pLRChild->bf){
                case LEFT_HIGH:
                    (*pTree)->bf=RIGHT_HIGH;
                    pLChild->bf=EQU_HIGH;
                    break;
                case EQU_HIGH:
                    (*pTree)->bf=EQU_HIGH;
                    pLChild->bf=EQU_HIGH;
                    break;
                case RIGHT_HIGH:
                    (*pTree)->bf=EQU_HIGH;
                    pLChild->bf=LEFT_HIGH;
                    break;
                default:
                    break;
            }
            pLRChild->bf=EQU_HIGH;
            left_rotate(&((*pTree)->pLeftChild));
            right_rotate(pTree);
            break;
        default:
            break;
    }
}

/*******************************************
* Make right tree balance
********************************************/
void right_balance(MY_STRUCT **pTree){
    MY_STRUCT *pRLChild;
    MY_STRUCT *pRChild;

    pRChild=(*pTree)->pRightChild;
    switch(pRChild->bf){
        case RIGHT_HIGH:
            (*pTree)->bf=EQU_HIGH;
            pRChild->bf=EQU_HIGH;
            left_rotate(pTree);
            break;
        case LEFT_HIGH:
            pRLChild=pRChild->pLeftChild;
            switch(pRLChild->bf){
                case LEFT_HIGH:
                    (*pTree)->bf=EQU_HIGH;
                    pRChild->bf=RIGHT_HIGH;
                    break;
                case EQU_HIGH:
                    (*pTree)->bf=EQU_HIGH;
                    pRChild->bf=EQU_HIGH;
                    break;
                case RIGHT_HIGH:
                    (*pTree)->bf=LEFT_HIGH;
                    pRChild->bf=EQU_HIGH;
                    break;
                default:
                    break;
            }
            pRLChild->bf=EQU_HIGH;
            right_rotate(&((*pTree)->pRightChild));
            left_rotate(pTree);
            break;
        default:
            break;
    }
}

/*******************************************
* Insert a new element into AVL tree
********************************************/
int insert_tree(MY_STRUCT **pTree, MY_STRUCT *pElem, int *pTaller){
    if(!(*pTree)){
        *pTree=pElem;
        (*pTree)->pLeftChild=NULL;
        (*pTree)->pRightChild=NULL;
        (*pTree)->bf=EQU_HIGH;
        *pTaller=1;
    }else{
        if(EQUIVALENT(pElem, *pTree)){
            *pTaller=0;
            return 0;
        }
        if(LOWER_THAN(pElem, *pTree)){
            if(!insert_tree(&((*pTree)->pLeftChild), pElem, pTaller)){
                return 0;
            }
            if(*pTaller){
                switch((*pTree)->bf){
                    case LEFT_HIGH:
                        left_balance(pTree);
                        *pTaller=0;
                        break;
                    case EQU_HIGH:
                        (*pTree)->bf=LEFT_HIGH;
                        *pTaller=1;
                        break;
                    case RIGHT_HIGH:
                        (*pTree)->bf=EQU_HIGH;
                        *pTaller=0;
                        break;
                    default:
                        break;
                }
            }
        }else{
            if(!insert_tree(&((*pTree)->pRightChild), pElem, pTaller)){
                return 0;
            }
            if(*pTaller){
                switch((*pTree)->bf){
                    case LEFT_HIGH:
                        (*pTree)->bf=EQU_HIGH;
                        *pTaller=0;
                        break;
                    case EQU_HIGH:
                        (*pTree)->bf=RIGHT_HIGH;
                        *pTaller=1;
                        break;
                    case RIGHT_HIGH:
                        right_balance(pTree);
                        *pTaller=0;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return 1;
}

/*******************************************
* Pre-order traverse of AVL tree
********************************************/
void pre_order_traverse(MY_STRUCT *pRoot){
    printf("%d\n", pRoot->order);
    if(pRoot->pLeftChild){
        pre_order_traverse(pRoot->pLeftChild);
    }
    if(pRoot->pRightChild){
        pre_order_traverse(pRoot->pRightChild);
    }
}

/*******************************************
* In-order traverse of AVL tree
********************************************/
void in_order_traverse(MY_STRUCT *pRoot){
    if(pRoot->pLeftChild){
        in_order_traverse(pRoot->pLeftChild);
    }
    //printf("%x\n", (unsigned int)pRoot);
    printf("\n");
    if(pRoot->pRightChild){
        in_order_traverse(pRoot->pRightChild);
    }
}

