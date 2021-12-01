#ifndef INCLUDED_GRAPHIC
#define INCLUDED_GRAPHIC

#include "boot_info.h"
#include "device/bios.h"

#define PALETTE_SIZE 16

typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} GraphicColor;

typedef GraphicColor GraphicPalette[PALETTE_SIZE];

#define PALETTE_NUMBER_ADDR 0x03c8
#define PALETTE_COLOR_ADDR 0x03c9

/**
  * @brief パレットの初期化を行う（基本一回しか行わないのでインライン関数としている）
  * @param [in] from            設定するパレットの番号
  * @param [in] to              設定するパレットの最大値
  * @param [in] originalPalette 設定するパレットの内容
  **/
void Graphic_SetPalette(unsigned int from, unsigned int to, GraphicPalette* originalPalette);

/**
  * @brief  指定した範囲を単色で塗りつぶす
  * @param  [out]    vram   VRAM
  * @param  [in]     xsize  画面の横サイズ
  * @param  [in]     color  塗りつぶす色
  * @param  [in]     x      始点座標x
  * @param  [in]     y      始点座標y
  * @param  [in]     width  横の幅
  * @param  [in]     height 縦の幅
  * @return 0
  */
unsigned char GraphicFillSquare2(
    unsigned char* vram,
    int xsize,
    unsigned char color,
    int x,
    int y,
    int width,
    int height
);

/**
  * @brief  指定した範囲を単色で塗りつぶす
  * @param  [out]    vram  VRAM
  * @param  [in]     xsize 画面の横サイズ
  * @param  [in]     color 塗りつぶす色
  * @param  [in]     x0    始点座標x
  * @param  [in]     y0    始点座標y
  * @param  [in]     x1    終点座標x
  * @param  [in]     y1    終点座標y
  * @return 0
  */
unsigned char GraphicFillSquare(
    unsigned char* vram,
    int xsize,
    unsigned char color,
    int x0,
    int y0,
    int x1,
    int y1
);

/**
  * @brief  画面を単色で塗りつぶす
  * @param  [in,out] boot_info 起動情報
  * @param  [in]     color     塗りつぶす色
  * @return GraphicFillSquare の戻り値
  */
unsigned char GraphicFillScreen(BootInfo* boot_info, unsigned char color);

void GraphicDrawImage(
    BootInfo* boot_info,
    int positionX,
    int positionY,
    int width,
    int height,
    char* image
);

/**
  * @brief  画面上に一文字出力する
  * @param  [in,out] boot_info 起動情報
  * @param  [in]     x         描画始点座標x
  * @param  [in]     y         描画始点座標y
  * @param  [in]     color     色
  * @param  [in]     charactor 文字
  * @return 0
  */
unsigned char GraphicPut(
    BootInfo* boot_info,
    int x,
    int y,
    const char color,
    const char charactor
);

/**
  * @brief  画面上に文字列を出力する
  * @param  [in,out] boot_info 起動情報
  * @param  [in]     x         描画始点座標x
  * @param  [in]     y         描画始点座標y
  * @param  [in]     color     色
  * @param  [in]     string    文字列
  * @return 0
  **/
unsigned char GraphicPuts(
    BootInfo* boot_info,
    int x,
    int y,
    char color,
    unsigned char* string
);


/**
  * @brief グラフィック周りを初期化する
  * @param [in] palette   グラフィックのパレット（16色）
  * @param [in] fillColor 最初に画面を塗りつぶす色
  **/
int Graphic_Initialize(
    GraphicPalette* palette,
    BootInfo* bootInfo,
    unsigned char fillColor
);

#endif
