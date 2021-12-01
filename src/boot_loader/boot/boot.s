
// Bios.h::BootInfoに格納する情報

// ブートセクタが読み込んだディスクの位置
.set    CYLS,   0x0ff0

// 起動時のキーボードのLEDの状態
.set    LEDS,   0x0ff1

// 色数
.set    VMODE,  0x0ff2

// 解像度
.set    SCRNX,  0x0ff4
.set    SCRNY,  0x0ff6

// グラフィックバッファの開始位置
.set    VRAM,   0x0ff8

// 画面モード
// 0x101: 640x480 (8bit color)
// .set    VBEMODE,    0x101
.set    VBEMODE,    0x000

// 固定アドレス値

// カーネルのロード先
.set KERNEL_ADDRESS,  0x00280000
.set RUN_KERNEL_ADDRESS, 0x0000001b

// ディスクキャッシュのアドレス
.set PROTECT_MODE_DISK_CACHE_ADDRESS, 0x00100000
.set REALMODE_DISK_CACHE_ADDRESS, 0x00008000

.text
.code16

check_exist_vbe:
    movw    $0x9000, %ax
    movw    %ax, %es
    movw    $0, %di
    movw    $0x4f00, %ax
    int     $0x10
    cmpw    $0x004f, %ax
    jne     screen320

check_vbe_version:
    movw    %es:4(%di), %ax
    cmpw    $0x0200, %ax
    jb      screen320

get_video_mode_info:
    movw    $VBEMODE, %cx
    movw    $0x4f01, %ax
    int     $0x10
    cmp     $0x004f, %ax
    jne     screen320

check_video_mode:
    cmpb    $0x08, %es:0x19(%di)
    jne     screen320
    cmpb    $0x04, %es:0x1b(%di)
    jne     screen320
    movw    %es:0x00(%di), %ax
    andw    $0x0080, %ax
    jz      screen320

change_video_mode:
    movw    $VBEMODE+0x4000, %bx
    movw    $0x4f02, %ax
    int     $0x10
    movb    $0x08, (VMODE)
    movw    %es:0x12(%di), %ax
    movw    %ax, (SCRNX)
    movw    %es:0x14(%di), %ax
    movw    %ax, (SCRNY)
    movl    %es:0x28(%edi), %eax
    movl    %eax, (VRAM)
    jmp     get_key_status

screen320:

    // Change video mode.
    movb    $0x13, %al
    movb    $0x00, %ah
    int     $0x10

    // Set screen information.
    movb    $8, (VMODE)
    movw    $320, (SCRNX)
    movw    $200, (SCRNY)
    movl    $0x000a0000, (VRAM)

get_key_status:

    // Get LED status.
    movb    $0x02, %ah
    int     $0x16
    movb    %al, (LEDS)


block_irq:
    movb $0xff, %al

    // PIC0_IMR に対して0xffを実行
    outb %al, $0x21

    // outを連続で実行するとうまくいかない機種があるため何もしない命令を挟む
    nop

    // PIC1_IMR に対して0xffを実行
    outb %al, $0xa1

    // CLIまでにPICに対して上記の操作を行っておくこと（でないとハングアップする可能性がある）
    cli

enable_a20gate:
    call get_data_impaction
    movb $0xd1, %al
    outb %al, $0x64
    call get_data_impaction

    // A20GATE信号線をONにする
    // ONになることで1MB以上のメモリ領域が扱えるようになる
    movb $0xdf, %al

    outb %al, $0x60
    call get_data_impaction

enable_protected_virtual_address_mode:
    // リアルアドレスモード：メモリアドレスの計算を行う際に、セグメントレジスタの値で直接アドレスの一部を指定する
    // プロテクトモード：メモリアドレスの計算を行う際に、実際のアドレスではないセグメント番号で仮想的に指定する。

    .arch i486
    lgdt (temporarily_GDT_information)
    movl %cr0, %eax
    andl $0x7fffffff, %eax

    // プロテクトモード移行
    orl $0x00000001, %eax
    movl %eax, %cr0

    // 機械語の解釈が変わるため、保護付きの32bitモードへの移行直後にjmp命令を実行する
    jmp flush_pipeline

flush_pipeline:

    // プロテクトモードへの移行に伴い、セグメントレジスタの値をGDT + 1の値にしておく
    movw $1*8, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

memcpy_kernel:
    movl $kernel, %esi
    movl $KERNEL_ADDRESS, %edi
    movl $512*1024/4, %ecx
    call memcpy

memcpy_boot_sector:
    movl $0x7c00, %esi
    movl $PROTECT_MODE_DISK_CACHE_ADDRESS, %edi
    movl $512/4, %ecx
    call memcpy

memcpy_cylinder:
    movl $REALMODE_DISK_CACHE_ADDRESS+512, %esi
    movl $PROTECT_MODE_DISK_CACHE_ADDRESS+512, %edi
    movl $0, %ecx
    movb (CYLS), %cl
    imull $512*18*2/4, %ecx
    sub $512/4, %ecx
    call memcpy

memcpy_data:
    movl $KERNEL_ADDRESS, %ebx
    movl 16(%ebx), %ecx
    addl $3, %ecx
    shrl $2, %ecx

    // 演算の結果が0だった場合は転送するものがないものとして次の処理へ移動する
    jz load_kernel

    // .data address
    movl 20(%ebx), %esi
    addl %ebx, %esi

    // .dataの転送先
    movl 12(%ebx), %edi
    call memcpy

load_kernel:
    movl 12(%ebx), %esp
    ljmpl $2*8, $RUN_KERNEL_ADDRESS

get_data_impaction:
    inb $0x64, %al
    andb $0x02, %al
    inb $0x60, %al
    jnz get_data_impaction
    ret

memcpy:
    movl (%esi), %eax
    addl $4, %esi
    movl %eax, (%edi)
    addl $4, %edi
    subl $1, %ecx
    jnz memcpy
    ret

.align 16

// 仮のGDTを構成する
// GDTの内容については segment.h::SegmentDescriptor を参照すること
generate_temporarily_GDT:
    .skip 8, 0x00
    .word 0xffff, 0x0000, 0x9200, 0x00cf
    .word 0xffff, 0x0000, 0x9a28, 0x0047
    .word 0x0000

// lgdtでgdtの場所を伝えるための情報
temporarily_GDT_information:
    .word 8 * 3 - 1
    .int generate_temporarily_GDT

.align 16

kernel:
