#include "graphic.h"
#include "font.h"

void Graphic_SetPalette(unsigned int from, unsigned int to, GraphicPalette* palette) {
    int eflags = _io_load_eflags();
    _io_cli();
    _io_out8(PALETTE_NUMBER_ADDR, from);
    for (unsigned int index = 0; index <= to; index++) {
        _io_out8(PALETTE_COLOR_ADDR, (*palette)[index].red / 4);
        _io_out8(PALETTE_COLOR_ADDR, (*palette)[index].green / 4);
        _io_out8(PALETTE_COLOR_ADDR, (*palette)[index].blue / 4);
    }
    _io_store_eflags(eflags);
    return;
}

unsigned char GraphicFillSquare2(
    unsigned char* vram,
    int xsize,
    unsigned char color,
    int x,
    int y,
    int width,
    int height
) {
    // TODO: 領域外に書き込まないようにバリデート処理を記述する
    for (int i = y; i < y+height; i++) {
        for (int j = x; j < x+width; j++) {
            vram[i * xsize + j] = color;
        }
    }
    return 0;
}

unsigned char GraphicFillSquare(
    unsigned char* vram,
    int xsize,
    unsigned char color,
    int x0,
    int y0,
    int x1, // widthにする
    int y1
) {
    // TODO: 領域外に書き込まないようにバリデート処理を記述する
    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            vram[y * xsize + x] = color;
        }
    }
    return 0;
}

unsigned char GraphicFillScreen(BootInfo* boot_info, unsigned char color) {
    return GraphicFillSquare(
        boot_info->vram,
        boot_info->screen_x,
        color, 0, 0,
        boot_info->screen_x-1,
        boot_info->screen_y-1
    );
}

void GraphicDrawImage(
    BootInfo* boot_info,
    int positionX,
    int positionY,
    int width,
    int height,
    char* image
) {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            boot_info->vram[(positionY + y) * boot_info->screen_x + (positionX + x)] = image[y*width+x];
        }
    }
}

unsigned char GraphicPut(
    BootInfo* boot_info,
    int x,
    int y,
    const char color,
    const char character
) {
    char data = 0x00;
    unsigned char *vram_pointer;

    for (int index = 0; index < 16; index++) {
        data = font_data[((character-0x20)*16)+index];
        vram_pointer = boot_info->vram + (y + index) * boot_info->screen_x + x;
        if ((data & 0x80) != 0) vram_pointer[0] = color;
        if ((data & 0x40) != 0) vram_pointer[1] = color;
        if ((data & 0x20) != 0) vram_pointer[2] = color;
        if ((data & 0x10) != 0) vram_pointer[3] = color;
        if ((data & 0x08) != 0) vram_pointer[4] = color;
        if ((data & 0x04) != 0) vram_pointer[5] = color;
        if ((data & 0x02) != 0) vram_pointer[6] = color;
        if ((data & 0x01) != 0) vram_pointer[7] = color;
    }

    return 0;
}

unsigned char GraphicPuts(
    BootInfo* boot_info,
    int x,
    int y,
    char color,
    unsigned char* string
) {
    for(int count = 0; *string != 0x00; string++, count++) {
        GraphicPut( boot_info, x + (count*8), y, color, *string );
    }

    return 0;
}

int Graphic_Initialize(
    GraphicPalette* palette,
    BootInfo* bootInfo,
    unsigned char fillColor
) {
    Graphic_SetPalette(0, 15, palette);
    GraphicFillScreen(bootInfo, fillColor);
    return 0;
}
