;文件名: kernel.c
;作者: 0xC000005
;日期: 2016/06/29
;作用: 内核模块

#include "../drivers/ports.h"

void main() {

    port_byte_out(0x3d4, 14);
    int position = port_byte_in(0x3d5);
    position = position << 8;

    port_byte_out(0x3d4, 15); 
    position += port_byte_in(0x3d5);
    int offset_from_vga = position * 2;
    char *vga = 0xb8000;
    vga[offset_from_vga] = 'X'; 
    vga[offset_from_vga+1] = 0x0f;
}
