#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#define PRIME 11
#define SIGNAL_NAME_LENGHTH 24

typedef int STATUS;

typedef struct port{
    char sig[SIGNAL_NAME_LENGHTH];
    void (*pSlot)(int x, ...);
    struct port *pNext;
}port_t;

STATUS connect(char *pSig, void *pSlot);
STATUS disconnect(char *pSig);
void emit(char *pSig, int argNum, ...);
void signal_slot_init(void);
int test_asm(int x, int y);

#endif
