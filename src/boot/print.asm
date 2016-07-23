;Filename : print.asm
;Author   : 0xC000005
;Website  : www.codeidea.cn
;Date     : 2016/06/29
;
;print    : 实模式下输出
;print_nl : 实模式下换行
;print_string_pm : 保护模式下输出

print:
    pusha


start:
    mov al, [bx]
    cmp al, 0
    je done

    mov ah, 0x0e
    int 0x10

    add bx, 1
    jmp start

done:
    popa
    ret

print_nl:
    pusha

    mov ah, 0x0e
    mov al, 0x0a
    int 0x10
    mov al, 0x0d
    int 0x10

    popa
    ret

[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_OB_BLACK equ 0x0f

print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY

print_string_pm_loop:
    mov al, [ebx]
    mov ah, WHITE_OB_BLACK

    cmp al, 0
    je print_string_pm_done

    mov [edx], ax
    add ebx, 1
    add edx, 2

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret
