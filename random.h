//*****************************************************************************
//
// random.h - Protoytpes for the random number generator.
// 
//*****************************************************************************

#ifndef __RANDOM_H__
#define __RANDOM_H__

//*****************************************************************************
//
// Prototypes for the random number generator functions.
//
//*****************************************************************************
extern void random_add_entropy(unsigned long ulEntropy);
extern void random_seed(void);
extern unsigned long random_number(void);

#endif // __RANDOM_H__
