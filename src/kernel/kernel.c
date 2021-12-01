#include "kernel.h"
#include "segment.h"
#include "graphic.h"
#include "cursor.h"
#include "device/pic.h"
#include "memory/memory.h"

#include "./../meimei/MEIMEI_Public.h"

// static void debug(BootInfo* bootInfo) {

//     // 記号を出力
//     GraphicFillSquare(bootInfo->vram, bootInfo->screen_x, 0x01, 70, 70, 80, 80);
//     GraphicFillSquare(bootInfo->vram, bootInfo->screen_x, 0x02, 90, 90, 100, 100);
//     GraphicFillSquare(bootInfo->vram, bootInfo->screen_x, 0x03, 110, 110, 120, 120);

//     // パレットを表示
//     for (int index = 0; index <= 0x0f; index++) {
//         // GraphicFillSquare(bootInfo->vram, bootInfo->screen_x, index,
//         //     bootInfo->screen_x-11, (index*11),
//         //     bootInfo->screen_x-1, (index*11)+10
//         // );
//         GraphicFillSquare2(bootInfo->vram, bootInfo->screen_x, index, bootInfo->screen_x-10, index*10, 10, 10);
//     }

//     // 一文字出力
//     GraphicPut(bootInfo, 10, 10, 0x07, 'H');

//     // 文字列を出力
//     char sample[100];
//     int number = 685391 * 2;
//     sprintf(sample, "sample: %d", number);
//     GraphicPuts(bootInfo, 20, 20, 0x07, (unsigned char*)sample);

//     char memory_string[100];
//     int i = memtest(0x00400000, 0xbfffffff) / (1024*1024);
//     sprintf(memory_string, "memory %dMB", i);
//     GraphicPuts(bootInfo, 20, 140, 0x07, (unsigned char*)memory_string);
// }

typedef struct {
    int key_data;
    char cursor[25];
    char cursor_fill[25];
    unsigned char keybuf[32];
    unsigned char mousebuf[128];
    Mouse mouse;
    int cursor_x;
    int cursor_y;
} Device;

static void initialize_device(Device* deviceObject) {
    for (int i = 0; i < 25; i++) deviceObject->cursor_fill[i] = 0x0c;
    Queue8_InitializeQueue8(&keyinfo, 32, deviceObject->keybuf);
    Queue8_InitializeQueue8(&mousefifo, 128, deviceObject->mousebuf);

    BootInfo* boot_info = BiosInitializeBootInfo();
    deviceObject->cursor_x = (boot_info->screen_x - 2) / 2;
    deviceObject->cursor_y = (boot_info->screen_y - 2) / 2;
}

Device device;

int Kernel_Status() {
    char string[40] = { 0 };
    // char string_before[40] = { 0 };
    // BootInfo* boot_info = BiosInitializeBootInfo();
    // int bottom_line = boot_info->screen_y-(16+1);

    _io_cli();
    if (Queue8_GetStatus(&keyinfo) != 0) {
        device.key_data = Queue8_Out(&keyinfo);
        _io_sti();
        if(__before_keydata != __keydata) __before_keydata = __keydata;
        __keydata = device.key_data;
        sprintf(string, "%d", __keydata);
        // sprintf(string_before, "%d", __before_keydata);
        // GraphicFillSquare(boot_info->vram, boot_info->screen_x, 0x02, 0, bottom_line, 8 * 4, boot_info->screen_y-1);
        // GraphicPuts(boot_info, 0, bottom_line, 0x07, (unsigned char*)string);
        // GraphicPuts(boot_info, 100, bottom_line, 0x07, (unsigned char*)string_before);
    } else if (Queue8_GetStatus(&mousefifo) != 0) {
        device.key_data = Queue8_Out(&mousefifo);
        _io_sti();

        if (Mouse_Decode(&(device.mouse), device.key_data) != 0) {
            // sprintf(string, "[lcr, %d, %d]", device.mouse.x, device.mouse.y);

            if ((device.mouse.button & 0x01) != 0) string[1]='L';
            if ((device.mouse.button & 0x02) != 0) string[1]='R';
            if ((device.mouse.button & 0x04) != 0) string[1]='C';

            // GraphicFillSquare(boot_info->vram, boot_info->screen_x, 0x03, 8 * 8, bottom_line, 8 * 24, boot_info->screen_y-1);
            // GraphicPuts(boot_info, 8*8, bottom_line, 0x07, (unsigned char*)string);

            // マウスカーソルの移動
            // まずはカーソルを消す
            // GraphicDrawImage(boot_info, device.cursor_x, device.cursor_y, 5, 5, device.cursor_fill);

            // カーソルの座標情報を更新
            device.cursor_x += device.mouse.x;
            device.cursor_y += device.mouse.y;

            // if(device.cursor_x < 0) device.cursor_x = 0;
            // if(device.cursor_y < 0) device.cursor_y = 0;
            // if(device.cursor_x > boot_info->screen_x) device.cursor_x = boot_info->screen_x;
            // if(device.cursor_y > boot_info->screen_y) device.cursor_y = boot_info->screen_y;

            // // 描画
            // GraphicDrawImage(boot_info, device.cursor_x, device.cursor_y, 5, 5, device.cursor);
        }

    } else {
        _io_stihlt();
    }
    return 1;
}

static void run_system() {
    BootInfo* boot_info = BiosInitializeBootInfo();
    // debug(boot_info);

    // Device device = { 0 };
    initialize_device(&device);

    // カーソルの表示
    CursorInitialize(device.cursor, 0x07, 0x00);
    GraphicDrawImage(boot_info, device.cursor_x, device.cursor_y, 5, 5, device.cursor);

    // 画面の塗りつぶし
    GraphicFillScreen(boot_info, 0x00);

    // Application
    loop MEIMEI_Main();
}

GraphicPalette palette = {
    { 0x00, 0x00, 0x00, },  // 黒
    { 0x00, 0x00, 0x86, },  // 青1
    { 0x00, 0x00, 0xff, },  // 青2
    { 0x00, 0x20, 0x20, },  // 緑1
    { 0x00, 0x40, 0x40, },  // 緑2
    { 0x00, 0x80, 0x80, },  // 緑3
    { 0x00, 0xA0, 0xA0, },  // 緑4
    { 0xff, 0xff, 0xff, },  // 白
    { 0x20, 0x00, 0x20, },  // 赤1
    { 0x40, 0x00, 0x40, },  // 赤2
    { 0x80, 0x00, 0x80, },  // 赤3
    { 0xA0, 0x00, 0xA0, },  // 赤4
    { 0x20, 0x20, 0x10, },  // 黄1
    { 0x40, 0x40, 0x20, },  // 黄2
    { 0x80, 0x80, 0x30, },  // 黄3
    { 0xA0, 0xA0, 0x40, },  // 黄4
};


static void initialize_system() {

    // Initialize.
    BootInfo* boot_info = BiosInitializeBootInfo();
    Segment_InitializeSegment();
    PIC_InitializePIC();
    Graphic_Initialize(&palette, boot_info, 0x00);

    // Enable device.
    PIC_AllowDevice();
}


int kernel_main() {
    initialize_system();
    run_system();
    return 0;
}
