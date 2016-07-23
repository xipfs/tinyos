;Filename : kernel_entry.asm
;Author   : 0xC000005
;Website  : www.codeidea.cn
;Date     : 2016/06/29
;
;将代码转移到由高级语言(c语言)来实现

global _start;
[bits 32]

_start:
    [extern kernel_main]
    call kernel_main
    jmp $
