#ifndef INCLUDED_KERNEL
#define INCLUDED_KERNEL

#define loop for (;;)
#define unless(formula) if(!(formula))

#include "queue.h"

Queue8 keyinfo;
Queue8 mousefifo;

int __keydata;
int __before_keydata;

int Kernel_Status();

#include <stdio.h>
#include <stdlib.h>

#endif
