;文件名: boot.asm
;作者: 0xC000005
;日期: 2016/06/29
;作用: 负责把主硬盘的MBR后X个扇区加载到0:0x1000,然后移交控制权。

org 0x7c00
KERNEL_OFFSET equ 0x1000
boot:
    mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp

    mov bx, MSG_REAL_MODE 
    call log16	; 记录日志
    call load_kernel	; 加载内核
    call jump_to_pm	; 跳转至保护模式
    jmp $

%include "boot/log.asm"	; 加载日志模块
%include "boot/disk.asm"	; 加载硬盘模块
%include "boot/pm.asm"	; 加载保护模式模块

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call log16

    mov bx, KERNEL_OFFSET
    mov dh, 16
    mov dl, [BOOT_DRIVE]
    call disk_load	; 加载硬盘数据
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call log32
    call KERNEL_OFFSET 
    jmp $


BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Enter 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

times 510 - ($-$$) db 0
dw 0xaa55
