;文件名: boot.asm
;作者: 0xC000005
;日期: 2016/06/29
;作用: 负责把主硬盘的MBR后X个扇区加载到0:0x1000,然后移交控制权。

[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ;

    mov [BOOT_DRIVE], dl ; 硬盘启动
    mov bp, 0x9000; 初始化栈
    mov sp, bp

    mov bx, MSG_REAL_MODE 
    call print
    call print_nl

    call load_kernel ; 从硬盘读取内核
    call switch_to_pm ; 关闭中断，加载GDT，最终跳转至保护模式
    jmp $
    
    
[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET ; 读取硬盘然后存储到内存 0x1000
    mov dh, 31 ; 
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET ; 将控制权交给内核
    jmp $ ; 等待内核交出控制权


BOOT_DRIVE db 0 ;
MSG_REAL_MODE db "开始进入 16-bit 实模式...", 0
MSG_PROT_MODE db "进入 32-bit 保护模式...", 0
MSG_LOAD_KERNEL db "加载内核到内存...", 0
MSG_RETURNED_KERNEL db "从内核返回...", 0

; padding
times 510 - ($-$$) db 0
dw 0xaa55