#include <stdio.h>

#include "bitmap.h"

int export_file(const char* exportFileName, const char* exportFileContents) {
    FILE *export_file = fopen(exportFileName, "w");
    if (export_file == NULL) {
        printf("[ERROR] File open error.\n");
        return 0;
    }

    fputs(exportFileContents, export_file);
    fclose(export_file);
    return 1;
}


int Maptip_bmpToMaptip(
    const char* bitmapFileName,
    const char* exportFileName,
    unsigned int separateXCount,
    unsigned int separateYCount
) {
    // FILE* fp = fopen( bitmapFileName, "rb" );
    BitmapObject bmp = {0};
    if (
        !LoadBitmap(bitmapFileName, &bmp) ||
        bmp.dip_header.width % separateXCount != 0 ||
        bmp.dip_header.height % separateYCount != 0
    ) {
        printf("[ERROR] Bitmap parse error.\n");
        return 0;
    }

    // BitmapData array[96][128] = {{0}};
    BitmapRGB** array = (BitmapRGB**)calloc(sizeof(BitmapRGB*), bmp.dip_header.height);
    for(int i = 0; i < bmp.dip_header.height; i++) {
        array[i] = (BitmapRGB*)calloc(sizeof(BitmapRGB), bmp.dip_header.width);
        if(array[i]==NULL) {
            printf("[ERROR] Calloc Bimap data error.");
            return 0;
        }
    }

    for(int y = bmp.dip_header.height - 1, yc=0; y>=0; y--, yc++) {
        for (int x = 0, xc=0; x < bmp.dip_header.width; x++, xc++) {
            int index = (y*bmp.dip_header.width) + x;
            array[yc][xc].red   = bmp.data.rgb[index].red;
            array[yc][xc].green = bmp.data.rgb[index].green;
            array[yc][xc].blue  = bmp.data.rgb[index].blue;
        }
    }

    // int iwidth = 16;
    int iwidth = bmp.dip_header.width / separateXCount;
    // int iheight = 6;
    int iheight = bmp.dip_header.height / separateYCount;

    // unsigned char maptip[16*6][16] = { {0} };
    int** maptip = (int**)calloc(sizeof(int*), iwidth*iheight);
    for(int i = 0; i < iwidth*iheight; i++) {
        maptip[i] = (int*)calloc(sizeof(int), separateYCount);
        if(maptip[i]==NULL) {
            printf("[ERROR] Calloc Maptip error.");
            return 0;
        }
        for(int j = 0; j < separateYCount; j++) {
            maptip[i][j] = 0x0;
        }
    }

    for(int iy = 0; iy < iheight; iy++) {
        for(int ix = 0; ix < iwidth; ix++) {

            // dot
            for (int y = 0; y < separateYCount; y++) {
                // printf("\n");
                for (int x = 0; x < separateXCount; x++) {
                    int point_x = (ix * separateXCount) + x;
                    int point_y = (iy * separateYCount) + y;
                    // printf(
                    //     "\e[48;2;%d;%d;%dm  \e[m",
                    //     array[point_y][point_x].red,
                    //     array[point_y][point_x].green,
                    //     array[point_y][point_x].blue
                    // );
                    if(
                        array[point_y][point_x].red == 0 &&
                        array[point_y][point_x].green == 0 &&
                        array[point_y][point_x].blue == 0
                    ) {
                        // printf("(iy*iwidth)+ix: %d, y:%d\n", (iy*iwidth)+ix, y);
                        maptip[(iy*iwidth)+ix][y] |= 1 << 7-x;
                    }
                }
            }
        }
    }

    // print
    unsigned char ascii_space_code = 0x20;
    char export_content[120000] = { 0 };
    sprintf(export_content, "%schar font_data[%d] = {\n", export_content, iwidth*iheight*separateYCount);
    // printf("static char font_data[%d] = {\n", iwidth*iheight*separateYCount);
    for(int i = 0;i<iwidth*iheight;i++) {
        sprintf(export_content, "%s    /* %c */ ", export_content, i+ascii_space_code);
        // printf("    /* %c */ ", i+ascii_space_code);
        for (int j = 0; j < separateYCount; j++) {
            sprintf(export_content, "%s0x%x", export_content, maptip[i][j]);
            // printf("0x%x", maptip[i][j]);
            if(j!=separateYCount-1) {
                sprintf(export_content, "%s, ", export_content);
                // printf(", ");
            }
        }
        if(i!=(iwidth*iheight)-1) {
            sprintf(export_content, "%s, ", export_content);
            // printf(", ");
        }
        sprintf(export_content, "%s\n", export_content);
        // printf("\n");
    }
    sprintf(export_content, "%s};\n", export_content);
    // printf("};\n");

    // export
    export_file(exportFileName, export_content);
    // printf("%s", export_content);

    CloseBitmap(bmp);
    // fclose(fp);
    return 1;
}
