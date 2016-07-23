;Filename : boot.asm
;Author   : 0xC000005
;Website  : www.codeidea.cn
;Date     : 2016/06/29
;
;负责把主硬盘的MBR后X个扇区加载到0:0x1000,然后移交控制权

[org 0x7c00]
KERNEL_OFFSET equ 0x1000

    mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print
    call print_nl

    call load_kernel
    call jump_to_pm
    jmp $

%include "boot/print.asm"
%include "boot/disk.asm"
%include "boot/pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET
    mov dh, 31
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET
    jmp $


BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0
MSG_RETURNED_KERNEL db "Returned from kernel. Error?", 0


times 510 - ($-$$) db 0
dw 0xaa55
