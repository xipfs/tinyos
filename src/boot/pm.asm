;文件名: pm.asm
;作者: 0xC000005
;日期: 2016/06/29
;作用: 定义了一个极简GDT，然后跳转到保护模式。

gdt_start:
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

gdt_code: 
    dw 0xffff
    dw 0x0  
    db 0x0    
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

[bits 16]
jump_to_pm:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm 

[bits 32]
init_pm:
    mov ax, DATA_SEG 
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM 

