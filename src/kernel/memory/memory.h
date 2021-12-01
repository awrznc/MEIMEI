#include "./../device/bios.h"
#include "flags_register.h"

#define CR0_CHACHE_DISABLE 0x60000000

unsigned int memtest_sub(unsigned int start, unsigned int end) {
    volatile unsigned int i;
    volatile unsigned int* p;
    volatile unsigned int old;
    volatile unsigned int pat0 = 0xaa55aa55;
    volatile unsigned int pat1 = 0x55aa55aa;

    for (i = start; i <= end; i += 0x1000) {
        p = (unsigned int *)i;
        old = *p;
        *p = pat0;

        *p ^= 0xffffffff;
        if (*p != pat1) {
            *p = old;
            break;
        }
        *p ^= 0xffffffff;
        if (*p != pat0) {
            *p = old;
            break;
        }
        *p = old;
    }

    return i;
}


/**
  * @brief CPUにキャッシュメモリが存在する場合は真値を返します。
  **/
static char is_ac_flag() {
    char result = 0;

    unsigned int extend_flag = 0;
    extend_flag = _io_load_eflags();

    extend_flag |= EFLAGS_AC_BIT;
    _io_store_eflags(extend_flag);
    extend_flag = _io_load_eflags();

    if ((extend_flag & EFLAGS_AC_BIT) != 0) result = 1;

    extend_flag &= ~EFLAGS_AC_BIT;
    _io_store_eflags(extend_flag);

    return result;
}


unsigned int memtest(unsigned int start, unsigned int end) {
    char cpu_is_486_over = is_ac_flag();

    if (cpu_is_486_over != 0) {
        unsigned int cr0 = _load_cr0();
        cr0 |= CR0_CHACHE_DISABLE; // キャッシュ禁止
        _store_cr0(cr0);
    }

    unsigned int i = memtest_sub(start, end);

    if (cpu_is_486_over != 0) {
        unsigned int cr0 = _load_cr0();
        cr0 &= ~CR0_CHACHE_DISABLE; // キャッシュ解除
        _store_cr0(cr0);
    }

    return i;
}

