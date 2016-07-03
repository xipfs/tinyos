;文件名: kernel_entry.asm
;作者: 0xC000005
;日期: 2016/06/29
;作用: 将代码转移到由高级语言(c语言)来实现.

[bits 32]
[extern main]
call main
jmp $
