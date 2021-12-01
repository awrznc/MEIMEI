.code32

.global _io_hlt
.global _io_cli, _io_sti, _io_stihlt
.global _io_in8, _io_in16, _io_in32
.global _io_out8, _io_out16, _io_out32
.global _io_load_eflags, _io_store_eflags
.global _write_mem8

.global _load_gdtr, _load_idtr
.global _load_cr0, _store_cr0

.global _asm_event21, _asm_event2c, _asm_event27

.section .text

_io_hlt:
    hlt
    ret

_io_cli:
    cli
    ret

_io_sti:
    sti
    ret

_io_stihlt:
    sti
    hlt
    ret

_io_in8:
    movl    4(%esp), %edx
    movl    $0, %eax
    inb     %dx, %al
    ret

_io_in16:
    movl    4(%esp), %edx
    movl    $0, %eax
    inw     %dx, %ax
    ret

_io_in32:
    movl    4(%esp), %edx
    inl     %dx, %eax
    ret

_io_out8:
    movl    4(%esp), %edx
    movl    8(%esp), %eax
    outb    %al, %dx
    ret

_io_out16:
    movl    4(%esp), %edx
    movl    8(%esp), %eax
    outw    %ax, %dx
    ret

_io_out32:
    movl    4(%esp), %edx
    movl    8(%esp), %eax
    outl    %eax, %dx
    ret

_io_load_eflags:
    pushfl
    pop     %eax
    ret

_io_store_eflags:
    movl    4(%esp), %eax
    push    %eax
    popfl
    ret

_write_mem8:
    movl    4(%esp), %ecx
    movb    8(%esp), %al
    movb    %al, (%ecx)
    ret

_load_gdtr:
    movw    4(%esp), %ax
    movw    %ax, 6(%esp)
    lgdt    6(%esp)
    ret

_load_idtr:
    movw    4(%esp), %ax
    movw    %ax, 6(%esp)
    lidt    6(%esp)
    ret

_load_cr0:
    movl    %cr0, %eax
    ret

_store_cr0:
    movl    4(%esp), %eax
    movl    %eax, %cr0
    ret

_asm_event21:
    push    %es
    push    %ds
    pushal
    movl    %esp, %eax
    push    %eax

    movw    %ss, %ax
    movw    %ax, %ds
    movw    %ax, %es
    call    PIC_Handler21

    pop     %eax
    popal
    pop     %ds
    pop     %es
    iret

_asm_event27:
    push    %es
    push    %ds
    pushal
    movl    %esp, %eax
    push    %eax

    movw    %ss, %ax
    movw    %ax, %ds
    movw    %ax, %es
    call    PIC_Handler27

    pop     %eax
    popal
    pop     %ds
    pop     %es
    iret

_asm_event2c:
    push    %es
    push    %ds
    pushal
    movl    %esp, %eax
    push    %eax

    movw    %ss, %ax
    movw    %ax, %ds
    movw    %ax, %es
    call    PIC_Handler2c

    pop     %eax
    popal
    pop     %ds
    pop     %es
    iret
