/*
* File name: kernel/kernel.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 内核程序
*
*/


#include <stdint.h>
#include "../lib/stdio.h"
#include "../lib/string.h"
#include "../lib/mem.h"
#include "../apps/games_2048.h"
#include "kernel.h"
#include "isr.h"
#include "../usr/shell.h"

int kernel_main() {
    isr_install();
    irq_install();

    asm("int $2");
    asm("int $3");

    shell();
    while(1);
    return 0;
}
