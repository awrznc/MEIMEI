#include "bitmap.h"

static void debug_print(BitmapObject* bmp) {

    // File header
    printf("type: %c%c\n", bmp->file_header.file_type[0], bmp->file_header.file_type[1]);
    printf("size: %ld\n", bmp->file_header.file_size[0]);
    printf("yoyaku1: %ld\n", bmp->file_header.yoyaku1[0]);
    printf("yoyaku2: %ld\n", bmp->file_header.yoyaku2[0]);
    printf("offset: %ld\n", bmp->file_header.offset[0]);

    // Information Header
    printf("size: %ld\n", bmp->dip_header.size);
    printf("height: %ld\n", bmp->dip_header.height);
    printf("width: %ld\n", bmp->dip_header.width);
    printf("bit: %ld\n", bmp->dip_header.bit);

    // for(int j = bmp->dip_header.height - 1; j>=0; j--) {
    //     for (int h = 0; h < bmp->dip_header.width; h++) {
    //         int i = (j*bmp->dip_header.width) + h;
    //         if(i % bmp->dip_header.width == 0) printf("\n");
    //         printf("\e[48;2;%d;%d;%dm  \e[m", bmp->data[i].red, bmp->data[i].green, bmp->data[i].blue);
    //     }
    // }
    return;
}

int LoadBitmap(const char* filepath, BitmapObject* bmp) {
    FILE* fp = fopen( filepath, "rb" );
    if ( fp == NULL) return 0;
    int result = ParseBitmap(fp, bmp);
    fclose(fp);
    return result;
}

int ParseBitmap(FILE* fp, BitmapObject* bmp) {
    if ( fp == NULL) return 0;

    // File header
    fread(&bmp->file_header, sizeof(bmp->file_header), 1, fp);
    if(bmp->file_header.file_type[0] != 'B' && bmp->file_header.file_type[1] != 'M') return 0;

    // Information Header
    fread(&bmp->dip_header, sizeof(bmp->dip_header), 1, fp);

    // Palette and Data
    if ( bmp->dip_header.bit == 1 ) {
        bmp->palette = (BitmapPalette*)calloc(sizeof(BitmapPalette), bmp->dip_header.pallet_color);
        for(int i = 0; i<bmp->dip_header.pallet_color; i++) fread(&bmp->palette[i], sizeof(bmp->palette[i]), 1, fp);

        unsigned long long datasize = (bmp->dip_header.width / 8) * bmp->dip_header.height;
        bmp->data.bit = (BitmapBit*)calloc(sizeof(BitmapBit), datasize);
        for(int i = 0; i<datasize; i++) fread(&bmp->data.bit[i], sizeof(bmp->data.bit[i]), 1, fp);

    } else if ( bmp->dip_header.bit == 24 ) {
        unsigned long long datasize = bmp->dip_header.width * bmp->dip_header.height;
        bmp->data.rgb = (BitmapRGB*)calloc(sizeof(BitmapRGB), datasize);
        for(int i = 0; i<datasize; i++) fread(&bmp->data.rgb[i], sizeof(bmp->data.rgb[i]), 1, fp);

    } else {
        printf("[ERROR] Unknown bit type: %d\n", bmp->dip_header.bit);
        return 0;
    }

    // print
    // debug_print(bmp);

    return 1;
}

int CloseBitmap(BitmapObject bmp) {
    if ( bmp.dip_header.bit == 1 ) {
        free(bmp.palette);
        free(bmp.data.bit);
    } else if ( bmp.dip_header.bit == 24 ) {
        free(bmp.data.rgb);
    } else {
        return 0;
    }
    return 1;
}
