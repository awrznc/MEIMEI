#include "stdlib.h"

static unsigned long int __random_value = 1;

static int __xorshift32( int v ) {
    v = v ^ ( v << 13 );
    v = v ^ ( v >> 17 );
    v = v ^ ( v << 15 );
    return v;
}

int rand() {
    return __random_value = __xorshift32(__random_value);
}

void srand( unsigned int seed ) {
    __random_value = seed;
}
