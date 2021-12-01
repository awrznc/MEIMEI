#ifndef INCLUDED_BOOT_INFO
#define INCLUDED_BOOT_INFO

#define BIOS_BOOT_ADDR 0x00000ff0

/**
  * @brief 起動情報を格納する構造体
  */
typedef struct _boot_info {
    char cyls;
    char leds;
    char video_mode;            // video mode
    char reserve;
    short screen_x, screen_y;   // 画面サイズ
    unsigned char *vram;        // video ram
} BootInfo;

/**
  * @brief 起動情報が格納されているメモリの先頭アドレスを利用して初期化を行う
  **/
static inline BootInfo* BiosInitializeBootInfo() {
    return (BootInfo*) BIOS_BOOT_ADDR;
}

#endif
