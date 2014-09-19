#ifndef _TREE_H_
#define _TREE_H_

#include "pool.h"

/*******************************************
* Macro definition
********************************************/
#define LEFT_HIGH   1
#define RIGHT_HIGH  -1
#define EQU_HIGH    0

#define EQUIVALENT(x1, x2)  (x1==x2)
#define LOWER_THAN(x1, x2)  (x1<x2)

#define MY_STRUCT buddy_t

/*******************************************
* Function prototype
********************************************/
int insert_tree(MY_STRUCT **pTree, MY_STRUCT *pElem, int *pTaller);
void pre_order_traverse(MY_STRUCT *pRoot);
void in_order_traverse(MY_STRUCT *pRoot);

#endif
