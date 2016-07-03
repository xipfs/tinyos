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
    vga[offset_from_vga] = 'T'; 
    vga[offset_from_vga+1] = 0x0f;
    vga[offset_from_vga+2] = 'i'; 
    vga[offset_from_vga+3] = 0x0f;
    vga[offset_from_vga+4] = 'n'; 
    vga[offset_from_vga+5] = 0x0f;
    vga[offset_from_vga+6] = 'y'; 
    vga[offset_from_vga+7] = 0x0f;
    vga[offset_from_vga+8] = 'O'; 
    vga[offset_from_vga+9] = 0x0f;
    vga[offset_from_vga+10] = 's'; 
    vga[offset_from_vga+11] = 0x0f;
    vga[offset_from_vga+12] = '!'; 
    vga[offset_from_vga+13] = 0x0f;
}
