#include <stdio.h>
#include "slot.h"
#include "signal.h"

void slot_test_a(int x){
    printf("This is slot a.\n");
}

void slot_test_b(int x){
    printf("This is slot b.\n");
}

void slot_add(int x, int y, int z){
    printf("%d+%d-%d=%d\n", x, y, z, (x+y-z));
}
