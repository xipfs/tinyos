/*
* File name: kernel/idt.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 构建中断描述符表
*
*/
#include "idt.h"
#include "type.h"

void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile("lidtl (%0)" : : "r" (&idt_reg));
}
