
/*
* File name: kernel/kernel.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 键盘处理
*
*/

#include "keyboard.h"
#include "ports.h"
#include "isr.h"
#include "../lib/stdio.h"
#include "../lib/string.h"
#include "../lib/function.h"
#include "../kernel/kernel.h"
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static void keyboard_callback(registers_t *regs) {
    unsigned char status;
    char keycode;

    port_byte_out(0x20, 0x20);

    status = port_byte_in(KEYBOARD_STATUS_PORT);
    if (status & 0x01) {
        keycode = port_byte_in(KEYBOARD_DATA_PORT);
        if(keycode < 0){
            onKeyUp((uint8_t)keycode-128);
            return;
        }
        onKeyDown(keycode);
    }

    UNUSED(regs);
}


void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback);
}
