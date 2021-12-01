#include <string.h>

#include "test.h"
#include "../src/bitmap.h"

void Test_LoadBitmap() {
    BitmapObject bmp = { 0 };
    assert_eq(LoadBitmap("./../../../share/1bit.bmp", &bmp), 1);

    assert_eq(bmp.file_header.file_type[0], 'B');
    assert_eq(bmp.file_header.file_type[1], 'M');

    assert_eq(bmp.file_header.file_size[0], 78);
    assert_eq(bmp.file_header.file_size[1], 0);
    assert_eq(bmp.file_header.file_size[2], 0);
    assert_eq(bmp.file_header.file_size[3], 0);

    assert_eq(bmp.file_header.offset[0], 62);
    assert_eq(bmp.file_header.offset[1], 0);
    assert_eq(bmp.file_header.offset[2], 0);
    assert_eq(bmp.file_header.offset[3], 0);

    assert_eq(bmp.dip_header.size, 40);
    assert_eq(bmp.dip_header.width, 8);
    assert_eq(bmp.dip_header.height, 4);
    assert_eq(bmp.dip_header.bit, 1);
    assert_eq(bmp.dip_header.comp, 0);
    assert_eq(bmp.dip_header.pallet_color, 2);

    assert_eq(bmp.palette[0].red, 0);
    assert_eq(bmp.palette[0].green, 0);
    assert_eq(bmp.palette[0].blue, 0);
    assert_eq(bmp.palette[1].red, 0xff);
    assert_eq(bmp.palette[1].green, 0xff);
    assert_eq(bmp.palette[1].blue, 0xff);

    assert_eq(bmp.data.bit[0].data[0], 0b10101010);
    assert_eq(bmp.data.bit[1].data[0], 0b01010101);
    assert_eq(bmp.data.bit[2].data[0], 0b10101010);
    assert_eq(bmp.data.bit[3].data[0], 0b01010101);

    CloseBitmap(bmp);
}

void Test_Load24Bitmap() {
    BitmapObject bmp = { 0 };
    assert_eq(LoadBitmap("./../../../share/font.bmp", &bmp), 1);

    assert_eq(bmp.file_header.file_type[0], 'B');
    assert_eq(bmp.file_header.file_type[1], 'M');

    int file_size = bmp.file_header.file_size[0] + (bmp.file_header.file_size[1] << 8);
    assert_eq(file_size, 36918);

    assert_eq(bmp.file_header.offset[0], 54);
    assert_eq(bmp.file_header.offset[1], 0);
    assert_eq(bmp.file_header.offset[2], 0);
    assert_eq(bmp.file_header.offset[3], 0);

    assert_eq(bmp.dip_header.size, 40);
    assert_eq(bmp.dip_header.width, 8*16);
    assert_eq(bmp.dip_header.height, 16*6);
    assert_eq(bmp.dip_header.bit, 24);
    assert_eq(bmp.dip_header.comp, 0);
    assert_eq(bmp.dip_header.pallet_color, 0);

    assert_eq(bmp.data.rgb[0].red, 255);
    assert_eq(bmp.data.rgb[0].green, 0);
    assert_eq(bmp.data.rgb[0].blue, 0);

    CloseBitmap(bmp);
}

int main() {
    _Static_assert(sizeof(i8) == 1, "Expecting 8 bit integers");
    _Static_assert(sizeof(u8) == 1, "Expecting 8 bit integers");
    _Static_assert(sizeof(i16) == 2, "Expecting 16 bit integers");
    _Static_assert(sizeof(u16) == 2, "Expecting 16 bit integers");
    _Static_assert(sizeof(i32) == 4, "Expecting 32 bit integers");
    _Static_assert(sizeof(u32) == 4, "Expecting 32 bit integers");

    Test_LoadBitmap();
    Test_Load24Bitmap();

    return 0;
}
