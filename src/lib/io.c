/*
* File name: lib/io.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 输入输出处理函数
*
*/

#include "io.h"
#include "stdio.h"
#include <stdint.h>

static char keycode = -1;


void onGetKeyFunction(char keycode1)
{
	keycode = keycode1;
}
int getch()
{
	char keycode1;
    registerListenKey(&onGetKeyFunction);
    while(keycode == -1);
    keycode1 = keycode;
    keycode = -1;
    return keycode1;
}
int getchInStep(uint32_t step)
{
	char keycode1;
    registerListenKey(&onGetKeyFunction);
    while(step--);
    keycode1 = keycode;
    keycode = -1;
    return keycode1;
}