/*
* File name: kernel/port.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 从特殊端口读写数据
*
*/

#include "ports.h"


uint8_t port_byte_in (uint16_t port) {
    uint8_t result;

    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out (uint16_t port, uint8_t data) {

    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in (uint16_t port) {
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out (uint16_t port, uint16_t data) {
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}
