#ifndef INCLUDED_BITMAP
#define INCLUDED_BITMAP

#include <stdio.h>
#include <stdlib.h>

#include "type.h"

struct {
    i8 file_type[2];
    u8 file_size[4];
    i8 yoyaku1[2];
    i8 yoyaku2[2];
    i8 offset[4];
} typedef BitmapFileHeader;

struct {
    u32 size;
    u32 width;
    u32 height;
    u16 plane_count;
    u16 bit;
    u32 comp;
    u32 image_size;
    u32 sh;
    u32 st;
    u32 pallet_color;
    u32 index;
} typedef DIBHeader;

typedef char ExtraBitMasks;

struct {
    u8 blue;
    u8 green;
    u8 red;
    u8 reserved;
} typedef BitmapPalette;

struct {
    u8 blue;
    u8 green;
    u8 red;
} typedef BitmapRGB;

typedef struct {
    u8 data[4];
} BitmapBit;

typedef struct {
    BitmapBit* bit;
    BitmapRGB* rgb;
} BitmapData;

struct {
    BitmapFileHeader file_header;
    DIBHeader dip_header;
    BitmapPalette *palette;
    BitmapData data;
} typedef BitmapObject;

/**
  * @brief BMPイメージを読み込んでパースします。
  **/
int LoadBitmap(const char* filepath, BitmapObject* bmp);

int ParseBitmap(FILE* fp, BitmapObject* bmp);

int CloseBitmap(BitmapObject bmp);

#endif
