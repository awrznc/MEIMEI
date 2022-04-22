/**
  * IPL: Initial program loader.
  **/

// === 16bit register ===
// ax: accumulator extend. アキュムレータ。累積演算器。
// cx: counter extend. カウンタ。数を数える機械。
// dx: data extend. データ。
// bx: base extend. 基点（土台）。
// sp: stack pointer. スタック用ポインタ。
// bp: base pointer. ベース用ポインタ。
// si: source index. 読み込みインデックス。
// di: destination index. 書き込みインデックス。

// extendはhighとlowの8bit registerで構成されている。
// 32bitに拡張する場合は、先頭にextendの`e`(例: eax)を付ける。
// 64bitに拡張する場合は、先頭に`r`(例: rax)を付ける。

// === segment register ===
// es: extra segment 1.
// fs: extra segment 2.
// gs: extra segment 3.
// cs: code segment.
// ss: stack segment.
// ds: data segment.

// === commands ===
// org: Origin. 機械語が実行時にどこに読み込まれるのか指定する命令。
// jmp: Jamp. 指定した場所に移動する命令。
// mov: Move. 代入命令。
// cmp: Compare. 比較命令。
// je:  Jump if Equal. 条件ジャンプ命令。
// int: Interrupt. ソフトウェア割込み命令(BIOSの機能が利用できる)。
// hlt: Halt. キーボードやマウスなどの外部機器が動くまでCPUを停止する。
// shr: Shift right. 右シフト命令。

// .movb: Move byte. 代入命令 ( 1 bytes )。
// .movw: Move word. 代入命令 ( 2 bytes )。
// .int:  Move integer. 代入命令 ( 4 bytes )。


// 16bitの機械語にアセンブルする
.code16

// Text section.
// プログラム本体や変更する必要のないデータをここに置く
.text

// FAT12

// Boot Sector and BPB Structure

// BS jmpBoot ( 3 bytes )
jmp     entry
.byte   0x90

// BS OEMName ( 8 bytes )
.ascii  "__MEIMEI"

// BPB BytsPerSec ( 2 bytes )
.word   512

// BPB SecPerClus ( 1 bytes )
.byte   1

// BPB RsvdSecCnt ( 2 bytes )
.word   1

// BPB NumFATs ( 1 bytes )
.byte   2

// BPB RootEntCnt ( 2 bytes )
.word   224

// BPB TotSec16 ( 2 bytes )
.word   2880

// BPB Media ( 1 bytes )
.byte   0xF0

// BPB FATSz16 ( 2 bytes )
.word   9

// BPB SecPerTrk ( 2 bytes )
.word   18

// BPB NumHeads ( 2 bytes )
.word   2

// BPB HiddSec ( 4 bytes )
.int    0

// BPB TotSec32 ( 4 bytes )
.int    2880


// Fat12 and Fat16 Structure Starting at Offset 36

// BS DrvNum ( 1 bytes )
.byte   0

// BS Reserved1 ( 1 bytes )
.byte   0

// BS BootSig ( 1 bytes )
.byte   0x29

// BS VolID ( 4 bytes )
.int    0xFFFFFFFF

// BS VolLab ( 11 bytes )
.ascii  "_____MEIMEI"

// BS FiSysType ( 8 bytes )
.ascii  "FAT12   "


// Empty.
.space  18


// Program.
.set    READ_IPL_ADDRESS, 0x7C00
.set    READ_DISK_ADDRESS, 0x0820
.set    MAX_RETRY_COUNT, 5
.set    MAX_SECTOR_COUNT, 18
.set    MAX_HEAD_COUNT, 2
.set    MAX_CYLINDER_COUNT, 10

entry:
    movw    $0, %ax
    movw    %ax, %ss
    movw    $READ_IPL_ADDRESS, %sp
    movw    %ax, %ds
    movw    %ax, %es

    // Set read address.
    movw    $READ_DISK_ADDRESS, %ax
    movw    %ax, %es

    // Cylinder ... 0
    movb    $0, %ch

    // Head ... 0
    movb    $0, %dh

    // Sector ... 2
    movb    $2, %cl

initialize_reader:

    // Fail counter.
    movw    $0, %si

read:

    // Set value of read disk function.
    movb    $0x02, %ah

    // Set read sector count.
    movb    $1, %al

    // Set read data to READ_DISK_ADDRESS.
    movw    $0, %bx

    // Set drive number. ( 0x00 : A Drive )
    movb    $0x00, %dl

    // Call BIOS.
    int     $0x13

    // 成功したらnextへ
    jnc     next

    // 失敗したらMAX_RETRY_COUNTの数だけ再実行する
    // それでもダメなら error とする
    add     $1, %al
    cmp     $MAX_RETRY_COUNT, %si
    jae     error

    movb    $0x00, %ah
    movb    $0x00, %dl
    int     $0x13
    jmp     read

next:

    // アドレスを 0x20 (512 bytes) 進める
    movw    %es, %ax
    add     $0x20, %ax
    movw    %ax, %es

    // Sector
    add     $1, %cl
    cmp     $MAX_SECTOR_COUNT, %cl
    jbe     initialize_reader
    movb    $1, %cl

    // Head
    add     $1, %dh
    cmp     $MAX_HEAD_COUNT, %dh
    jb      initialize_reader
    movb    $0, %dh

    // Cylinder
    add     $1, %ch
    cmp     $MAX_CYLINDER_COUNT, %ch
    jb      initialize_reader

    movb    %ch, (0x0ff0)
    jmp     0xC200

error:
    movw    $message, %si

loop:
    movb    (%si), %al
    add     $1, %si
    cmp     $0, %al
    je      finish

    // Set value of print character function.
    movb    $0x0E, %ah

    // Set color code.
    movw    $15, %bx

    // Call BIOS.
    int     $0x10
    jmp     loop

finish:
    hlt
    jmp finish

message:
    .string "\n\nRead error.\n"

    .org    0x1FE
    .byte   0x55, 0xAA
