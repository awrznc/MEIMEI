#include "stage.h"

#define MEIMEI_EFFECT_BREAK_IMAGE_SIZE 11

char break_effect[MEIMEI_EFFECT_BREAK_IMAGE_SIZE * MEIMEI_STAGE_WIDTH / 8 * MEIMEI_STAGE_HEIGHT] = {

    // default
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00111100, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00111100, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,

    // 1-1
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b11111111, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b11111111, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b11111111, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b11111111, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,

    // 1-2
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000011, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00000011, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00001111, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00001111, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00000011, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00000011, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,

    // 2-1
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000011, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00001111, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00111111, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b11111111, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b11111111, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00111111, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00001111, 0b11111111, 0b11000000, 0b00000000,
    0b00000000, 0b00000011, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,

    // 2-2
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11000000, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11000000, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11000000, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11000000, 0b00000000,
    0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,

    // 3-1
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b11111111, 0b11111111, 0b00000000, 0b11000000, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11111100, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11000000,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11000000,
    0b11111111, 0b11111111, 0b11111111, 0b11111100, 0b00000000,
    0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000,
    0b11111111, 0b11111111, 0b00000000, 0b11000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,

    // 3-2
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b11111111,
    0b11111111, 0b11111111, 0b00011000, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b00000000, 0b11111111, 0b11111111,
    0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b11111111,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,

    // 4-1
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b11111111, 0b00000000, 0b00011000, 0b00000000, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b00000000, 0b11111111, 0b11111111,
    0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b11111111,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,

    // 5-1
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b00000000, 0b00011000, 0b00000000, 0b11111111,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,

    // 5-2
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111
};

int is_draw(unsigned int x, unsigned int y, int level) {
    if (MEIMEI_EFFECT_BREAK_IMAGE_SIZE-1 < level) return 0;
    return break_effect[(MEIMEI_STAGE_WIDTH/8*MEIMEI_STAGE_HEIGHT*level)+y*(MEIMEI_STAGE_WIDTH/8)+(x/8)] & ( 0b10000000 >> (x%8) );
}