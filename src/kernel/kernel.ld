OUTPUT_FORMAT("binary");

DATA_ADDRESS = 0x310000;

SECTIONS {
    .head 0x0 : {
        /* Data segment size. */
        LONG(64 * 1024);

        /* Signature. ("_MEI") */
        LONG(0x49454D5F);

        /* Stack area in Data segment. */
        LONG(0);

        /* Data address in memory. */
        LONG(DATA_ADDRESS);

        /* Data section size. */
        LONG(SIZEOF(.data));

        /* Data address in file. */
        LONG(LOADADDR(.data));

        /* Application address. */
        LONG(0xE9000000);

        /* `Entry point (kernel_main)` - 0x20 */
        LONG(kernel_main - 0x20);

        /* Heap area address. */
        LONG(0);
    }

    /* プログラム本体 */
    .text : { *(.text) }

    /* ATでセクションのロードアドレスと再配置アドレスの値を揃える */
    .data DATA_ADDRESS : AT( ADDR(.text) + SIZEOF(.text) ) {

        /* 定数値 (read only) */
        *(.rodata*);

        /* 初期値が存在するグローバル変数 */
        *(.data);

        /* 初期値なしグローバル変数 */
        *(.bss);
    }

    /* バックトレースを取得するための特別なセクション */
    /DISCARD/ : { *(.eh_frame) }
}
