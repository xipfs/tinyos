/*
* File name: kernel/system.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 系统功能
*
*/

#include "system.h"
#include "ports.h"
#include "../lib/stdio.h"


unsigned read_cmos(unsigned reg, char bcd)
{
	unsigned high_digit, low_digit;

	port_byte_out(0x70, reg);
	high_digit = low_digit = port_byte_in(0x71);
	if(!bcd)
		return low_digit;
	high_digit >>= 4;
	high_digit &= 0x0F;
	low_digit &= 0x0F;
	return 10 * high_digit + low_digit;
}


void reboot()
{
	kprint("System rebooting...\n");
	port_byte_out(0x64, 0xFE);
	kprint("Sorry, reboot faild, please press the reset button.\n");
}