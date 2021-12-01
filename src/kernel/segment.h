#ifndef INCLUDED_SEGMENT
#define INCLUDED_SEGMENT

#include "device/bios.h"

/**
  * GDT (global segment descriptor table) -- 大域セグメント記述子表
  *
  * メモリのどこかにセグメント(切り分けたメモリのブロック)の設定。
  * セグメントの先頭の番地と有効設定個数をCPUのCPUのGDTR(GDT Register)に設定する。
  **/
typedef struct _segment_descriptor {
    /**
      * Limit -- セグメントの大きさ (Segment size)
      * セグメント属性の値を1にすると単位がbyte単位からページ単位に切り替わる。
      **/
    short limit_low;

    /** Address -- セグメントの番地 **/
    short base_low;

    /** Address 2 **/
    char base_mid;

    /**
      * Access -- セグメント属性 (Segment attribute)
      * セグメント属性の下位8bitを使ってメモリ空間のカテゴリー化を行う（割り込み種類識別用）
      **/
    char access_right;

    /** Limit 2 **/
    char limit_high;

    /** Address 3 **/
    char base_high;
} SegmentDescriptor;


static inline void set_segment_descriptor(
    SegmentDescriptor* gdt,
    unsigned int limit,
    int base,
    int ar                  // Access right.
) {
    for (int i = 0; limit > 0xfffff; i++) {
        ar |= 0x8000;
        limit /= 0x1000;
    }

    // Limit
    gdt->limit_low  = limit & 0xffff;
    gdt->limit_high = ((limit >> 16) & 0xffff) | ((ar >> 8) & 0xf0);

    // Access
    gdt->access_right = ar & 0xff;

    // Base
    gdt->base_low  = base & 0xffff;
    gdt->base_mid  = (base >> 16) & 0xff;
    gdt->base_high = (base >> 24) & 0xff;

    return;
}

#define LIMIT_GDT 0x0000ffff

static inline void init_gdt(SegmentDescriptor* gdt) {
    for(int i=0; i<8192; i++) {
        set_segment_descriptor(gdt + i, 0, 0, 0);
    }
    set_segment_descriptor(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
    set_segment_descriptor(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
    _load_gdtr(0xffff, 0x00270000);

    return ;
}

/**
  * IDT (interrupt descriptor table) -- 割り込み記述子表
  *
  * 割り込みゲートディスクリプタ。
  * 割り込みの設定。
  **/
typedef struct _gate_descriptor {
    short offset_low;   /** 割り込み時の実行アドレス(下位16bit) **/
    short selector;     /** セグメントセレクタ **/
    char dw_count;      /** 予約領域 **/
    char access_right;  /** セグメント属性 **/
    short offset_high;  /** 割り込み時の実行アドレス(上位16bit) **/
} GateDescripter;

static inline void set_gate_descriptor(GateDescripter* idt, int offset, int selector, int ar) {
    idt->selector     = selector;
    idt->dw_count     = (ar >> 8) & 0xff;
    idt->access_right = ar & 0xff;

    // Address
    idt->offset_low  = offset & 0xffff;
    idt->offset_high = (offset >> 16) & 0xffff;
    return;
}

// IDTに対する属性の設定
#define AR_INTGATE32 0x008e

static inline void init_idt(GateDescripter* idt) {
    for (int i = 0; i < 256; i++) {
        set_gate_descriptor(idt + i, 0, 0, 0);
    }

    set_gate_descriptor(idt+0x21, (int)_asm_event21, 2*8, AR_INTGATE32);
    set_gate_descriptor(idt+0x27, (int)_asm_event27, 2*8, AR_INTGATE32);
    set_gate_descriptor(idt+0x2c, (int)_asm_event2c, 2*8, AR_INTGATE32);

    _load_idtr(0x7ff, 0x0026f800);

    return;
}


#define ADDRESS_GDT 0x00270000
#define ADDRESS_IDT 0x0026f800

typedef struct _segment {
    SegmentDescriptor* gdt;
    GateDescripter* idt;
} Segment;


/**
  * @brief GDTとIDTを初期化する
  **/
void Segment_InitializeSegment() {
    Segment segment = {
        (SegmentDescriptor *)ADDRESS_GDT,
        (GateDescripter *)ADDRESS_IDT
    };

    init_gdt(segment.gdt);
    init_idt(segment.idt);
}

#endif
