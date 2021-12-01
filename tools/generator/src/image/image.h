#ifndef INCLUDED_IMAGE
#define INCLUDED_IMAGE

#include "bitmap.h"
#include "./../header/header.h"

#define BYTE_SIZE 8

int Image_GenerateBinaryFile( const char* importFileName, const char* exportFileName ) {
    BitmapObject bmp = {0};
    if ( !LoadBitmap(importFileName, &bmp) ) return 0;

    u32 width = bmp.dip_header.width;
    u32 height = bmp.dip_header.height;
    u32 content_size = height*(width/BYTE_SIZE);
    char* export_content = (char*)calloc(sizeof(char), content_size);
    if(export_content == NULL) {
        printf("[ERROR] Calloc Maptip error.");
        return 0;
    }
    u32 bitmap_block_size = height*(width/(sizeof(BitmapBit)*BYTE_SIZE));
    for (int i = 0; i < bitmap_block_size; i++) {
        for (int j = 0; j < sizeof(BitmapBit); j++) {
            export_content[(i*sizeof(BitmapBit))+j] = bmp.data.bit[i].data[j];
        }
    }

    FILE *export_file = fopen(exportFileName, "w");
    Header_WriteFile(export_file, importFileName, export_content, content_size);
    fclose(export_file);
    free(export_content);

    return 1;
}

#endif
