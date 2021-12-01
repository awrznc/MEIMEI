#ifndef INCLUDED_BIOS
#define INCLUDED_BIOS

/**
  * @brief HLT (Halt) 命令。 割り込みが発生するまでCPUを休ませる。
  **/
void _io_hlt(void);

/**
  * @brief CLI (Clear interrupt flag) 命令。CPUが外部装置の命令を受け付けないようにする。
  **/
void _io_cli(void);

/**
  * @brief STI (Set interrupt flag) 命令。CPUが外部装置の命令を受け付けるようにする。
  **/
void _io_sti(void);

void _io_stihlt();

int _io_in8(int port);

void _io_out8(int port, int data);
int _io_load_eflags(void);
void _io_store_eflags(int eflags);

void _load_gdtr(int limit, int addr);
void _load_idtr(int limit, int addr);
int _load_cr0();
void _store_cr0(int cr0);

void _asm_event21();
void _asm_event27();
void _asm_event2c();


#endif
