/**
  * PIC (Proggrammable Interrupt Controller)
  * ===
  *
  * 設定可能な割り込みコントローラ。
  *
  * 単独では1個の割り込みしか行えないCPUの機能を補うために追加されたチップ。
  * 8つの割り込み信号を1つの割り込み信号にまとめる装置（8259Aの場合）
  *
  *
  * ## ISA (Industry Standard Architecture)
  *
  * 業界標準のアーキテクチャ。
  *
  *
  * ## IRQ (Interrupt Request)
  *
  * 割り込み要求。
  *
  * 割り込み要求の信号を15個扱う場合はIRQ2を通して利用する。
  * これは基板上の配線であるため、ソフトウェア側で変更することはできない。
  *
  * ~~~
  * +-----+    +----------------+        +----------------+
  * | CPU +----+  PIC (master)  |   +----+  PIC (slave)   |
  * +-----+    ++++++++++++++++++   |    ++++++++++++++++++
  *             ||||||||            |     ||||||||
  *             |||||||+-- IRQ0     |     |||||||+-- IRQ8
  *             ||||||+-- IRQ1      |     ||||||+-- IRQ9
  *             |||||+-- IRQ2 ------+     |||||+-- IRQ10
  *             ||||+-- IRQ3              ||||+-- IRQ11
  *             |||+-- IRQ4               |||+-- IRQ12
  *             ||+-- IRQ5                ||+-- IRQ13
  *             |+-- IRQ6                 |+-- IRQ14
  *             +-- IRQ7                  +-- IRQ15
  * ~~~
  *
  *
  * ## IMR (Interrrupt Mask Register)
  *
  * 割り込みマスクレジスタ。
  *
  * IRQに目隠しを行い割り込みをブロックする。
  * 例えば、以下のような場合に目隠しをおこなう。
  *
  * - 割り込みに関する設定を操作する際、割り込みを受け付けたくない場合
  * - 対象のIRQに何も装置が繋がっておらず、静電気などの影響を受ける懸念がある場合
  *
  *
  * ## ICW (Initial Control Words)
  *
  * 初期化制御データ。
  *
  * ICWには1~4が存在する。
  *
  * ICW1とICW4には、「PICの基板上での配線」や「割り込み信号の電気的な特性」に関する設定を行う。
  * 変な値を設定するとPCが物理的に壊れる可能性があるため、細心の注意をはらう必要がある。
  *
  * ICW3には、「masterとslaveの接続」に関する設定を行う。
  * masterに対しては、何番のIRQにslaveが繋がっているかを8bitで設定する。
  * 例えば、IRQ2のみslaveが繋がっていることを示す場合は`00000100`を設定する。
  *
  * ICW2には、「CPUに通知するためのIRQの割り込み番号」に関する設定を行う。
  * 他のICWはハードウェアの構成に依存するため、ソフトウェア側でいじれるのは実質このICW2のみとなる。
  *
  *
  * ## OCW (Operation Command Words)
  *
  * 操作コマンドデータ。
  *
  * OCWには1~3が存在する。
  *
  * OCW1には、IMRに格納したい値を設定する。
  * 例えば、IRQ1に割り込みが入らないようにしたい場合は`00000010`を設定する。
  *
  * OCW2には、PICの操作を設定します。
  *
  **/

#ifndef INCLUDED_PIC
#define INCLUDED_PIC

#include "../kernel.h"
#include "bios.h"
#include "queue.h"

// PIC I/O Port (master)
#define PIC0_OCW2 0x0020
#define PIC0_ICW1 0x0020
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021
#define PIC0_IMR  0x0021

// PIC I/O Port (slave)
#define PIC1_OCW2 0x00a0
#define PIC1_ICW1 0x00a0
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1
#define PIC1_IMR  0x00a1

// PIC Number
#define PIC0_IRQ0 0x0060        // Timer
#define PIC0_IRQ1 PIC0_IRQ0 + 1 // Key Board
#define PIC0_IRQ2 PIC0_IRQ0 + 2 // Cascaded Signals (IRQ8~15)
#define PIC0_IRQ7 PIC0_IRQ0 + 7 // Printers or Any Parallel Port.
#define PIC1_IRQ4 PIC0_IRQ0 + 4 // Mouse (IRQ12)

// Keyboard I/O Port
#define PIC_KEYBOARD_IO_DATA_PORT 0x0060                    // read
#define PIC_KEYBOARD_IO_OUTPUT_DATA 0x0060                  // write
#define PIC_KEYBOARD_IO_STATUS 0x0064                       // read
#define PIC_KEYBOARD_IO__OUTPUT_CONTROLL_COMMAND 0x0064     // write

// Mask (PIC_KEYBOARD_IO_STATUS)
#define PIC_ALLOW_RECEIVE_MASK 0x02

// Command (PIC_KEYBOARD_IO__OUTPUT_CONTROLL_COMMAND)
#define PIC_SET_MODE_COMMAND 0x0060
#define PIC_SEND_DATA_TO_MOUSE_COMMAND 0xd4

// Command (PIC_KEYBOARD_IO_DATA_PORT)
#define PIC_USE_MOUSE_COMMAND 0x47
#define PIC_ENABLE_MOUSE_COMMAND 0xf4

/**
  * @brief PICを初期化する
  **/
void PIC_InitializePIC() {

    // 全ての割り込みを受け付けない
    _io_out8(PIC0_IMR, 0xff);
    _io_out8(PIC1_IMR, 0xff);

    // エッジトリガモード
    _io_out8(PIC0_ICW1, 0x11);

    // IRQ0-7はINT20-27で受け取る
    _io_out8(PIC0_ICW2, 0x20);

    // PIC1はIRQ2にて接続
    _io_out8(PIC0_ICW3, 1 << 2);

    // ノンバッファモード
    _io_out8(PIC0_ICW4, 0x01);

    // エッジトリガーモード
    _io_out8(PIC1_ICW1, 0x11);

    // IRQ8-15はINT28-2f
    _io_out8(PIC1_ICW2, 0x28);

    // PIC1はIRQ2にて接続
    _io_out8(PIC1_ICW3, 2);

    // ノンバッファモード
    _io_out8(PIC1_ICW4, 0x01);

    // PIC1以外はすべて禁止
    _io_out8(PIC0_IMR, 0xdb);

    // 全ての割り込みを受け付けない
    _io_out8(PIC1_IMR, 0xff);
}


#define MOUSE_SIGNAL_PREFIX 0xfa
#define MOUSE_RECEIVE_PHASE_COUNT 3

typedef struct _mouse {
    unsigned char data[MOUSE_RECEIVE_PHASE_COUNT];
    int x; // 移動情報x
    int y; // 移動情報y
    int button; // ボタンの状態
    unsigned char current_phase;
} Mouse;

#define Mouse_InitiaizeMouse() { 0 }

/**
  * @brief マウスの情報をデコードする。
  * @param [in,out] mouse マウスの情報
  * @param [in] data マウスの情報に格納する情報
  * @return 0: マウスからの受信が未完了, 1: マウスからの受信が完了, -1: 意図しない状態
  **/
int Mouse_Decode(Mouse* mouse, unsigned char data) {

    switch (mouse->current_phase) {
        case 0: {
            if (data == MOUSE_SIGNAL_PREFIX) mouse->current_phase = 1;
            return 0;
        }
        case 1: {

            // 128bitと64bitに不正なフラグが立っていないかチェック
            // もし立っていた場合は不正なデータと判断して破棄する
            if ((data & 0b11001000) == 0b00001000) {
                mouse->data[0] = data;
                mouse->current_phase = 2;
            }
            return 0;
        }
        case 2: {
            mouse->data[1] = data;
            mouse->current_phase = 3;
            return 0;
        }
        case 3: {
            mouse->data[2] = data;
            mouse->current_phase = 1;

            // ボタンの情報はdataの下位3bitに存在する
            mouse->button = mouse->data[0] & 0b00000111;
            mouse->x = mouse->data[1];
            mouse->y = mouse->data[2];

            // dataの1バイト目からマウス情報の整形を行う
            // 16bit目にフラグが立っていた場合はxの値の8bitより上をすべて1にする
            // 32bit目にフラグが立っていた場合はyの値の8bitより上をすべて1にする
            if ((mouse->data[0] & 0b00010000) != 0) mouse->x |= 0xffffff00;
            if ((mouse->data[0] & 0b00100000) != 0) mouse->y |= 0xffffff00;

            // マウスのy方向と画面の方向は逆であるため、ここで合わせる
            mouse->y *= -1;

            return 1;
        }
    }

    return -1;
}


/**
  * @brief デバイス（キーボードとマウス）の制御回路が制御命令を受け付けられるまで待つ。
  **/
static void wait_for_device_to_ready() {
    loop if ((_io_in8(PIC_KEYBOARD_IO_STATUS) & PIC_ALLOW_RECEIVE_MASK) == 0) break;
}


/**
  * @brief デバイス（キーボードとマウス）の制御回路を初期化する。
  **/
static void initialize_device_controller() {
    wait_for_device_to_ready();
    _io_out8(PIC_KEYBOARD_IO__OUTPUT_CONTROLL_COMMAND, PIC_SET_MODE_COMMAND);
    wait_for_device_to_ready();
    _io_out8(PIC_KEYBOARD_IO_DATA_PORT, PIC_USE_MOUSE_COMMAND);
}


/**
  *  @brief マウスを有効化する。マウス用の回路はキーボード用の回路よりずっと後に実装されたため、有効化する必要がある。
  **/
static void enable_mouse() {
    wait_for_device_to_ready();
    _io_out8(PIC_KEYBOARD_IO__OUTPUT_CONTROLL_COMMAND, PIC_SEND_DATA_TO_MOUSE_COMMAND);
    wait_for_device_to_ready();
    _io_out8(PIC_KEYBOARD_IO_DATA_PORT, PIC_ENABLE_MOUSE_COMMAND);
}


/**
  * @brief デバイスによる割り込みを許可・有効化する
  **/
int PIC_AllowDevice() {

    // Allow commands from the device.
    _io_sti();

    // Allow PIC1 and Keyboard.
    _io_out8(PIC0_IMR, 0b11111001);

    // Allow Mouse.
    _io_out8(PIC1_IMR, 0b11101111);

    initialize_device_controller();
    enable_mouse();

    return 0;
}


/**
  * @brief IRQの監視を再開する。
  * @param [in] picNumber 監視を再開するPICの番号。
  * @param [in] irqNumber 監視を再開するIRQの番号。IRQ1(keyboard on PS/2 port)の監視を再開させたい場合は`1`を渡す。
  **/
inline static void remonitoring(int picNumber, int irqNumber) {
    _io_out8(picNumber, irqNumber);
}


/**
  * @brief キーコードを取得する。
  * @note キーを押した際の値を以下に示す。（フルキーの値のみ）
  *
  * 00: None   10: Q      20: D      30: B      40: F6     50: PageDn 60:        70:
  * 01: ESC    11: W      21: F      31: N      41: F7     51:        61:        71:
  * 02: 1      12: E      22: G      32: M      42: F8     52: Ins    62:        72:
  * 03: 2      13: R      23: H      33: ,      43: F9     53: Del    63:        73: _
  * 04: 3      14: T      24: J      34: .      44: F10    54: SysReq 64:        74:
  * 05: 4      15: Y      25: K      35: /      45: NL     55:        65:        75:
  * 06: 5      16: U      26: L      36: RShift 46: SL     56:        66:        76:
  * 07: 6      17: I      27: ;      37:        47:        57: F11    67:        77:
  * 08: 7      18: O      28: :      38: Alt    48: PageUp 58: F12    68:        78:
  * 09: 8      19: P      29:        39: Space  49:        59:        69:        79: convert to Kanji
  * 0A: 9      1A: @      2A: LShift 3A: CL     4A:        5A:        6A:        7A:
  * 0B: 0      1B: [      2B: ]      3B: F1     4B: Home   5B: LCM    6B:        7B: no convert to Kanji
  * 0C: -      1C: Enter  2C: Z      3C: F2     4C:        5C:        6C:        7C:
  * 0D: ^      1D: Ctrl   2D: X      3D: F3     4D: End    5D: RCM    6D:        7D: \
  * 0E: BS     1E: A      2E: C      3E: F4     4E:        5E:        6E:        7E:
  * 0F: Tab    1F: S      2F: V      3F: F5     4F:        5F:        6F:        7F:
  *
  **/
inline static int get_key_code() {
    return _io_in8(PIC_KEYBOARD_IO_DATA_PORT);
}

#define UNUSED(__esp) ((void)__esp)

void PIC_Handler21(int* __esp) {
    UNUSED(__esp);

    remonitoring(PIC0_OCW2, PIC0_IRQ1);

    unsigned char data = 0;
    data = get_key_code();
    Queue8_In(&keyinfo, data);
}

/**
  * @brief PS/2マウスからの割り込み
  **/
void PIC_Handler2c(int* __esp) {
    UNUSED(__esp);

    remonitoring(PIC1_OCW2, PIC1_IRQ4);
    remonitoring(PIC0_OCW2, PIC0_IRQ2);

    unsigned char data = 0;
    data = get_key_code();
    Queue8_In(&mousefifo, data);
}

// 一部の機器から送信されるノイズの処理
void PIC_Handler27(int* __esp) {
    UNUSED(__esp);

    _io_out8(PIC0_OCW2, PIC0_IRQ7);
}

#endif
