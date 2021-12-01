/**
  * FLAGS register.
  * FLAGSレジスタは、CPUの現在の状態を格納したステータスレジスタです。
  */

// EFLAGS
#define EFLAGS_RF_BIT 0x00010000
#define EFLAGS_VM_BIT 0x00020000
#define EFLAGS_AC_BIT 0x00040000
#define EFLAGS_VIF_BIT 0x00080000
#define EFLAGS_VIP_BIT 0x00100000
#define EFLAGS_ID_BIT 0x00200000
