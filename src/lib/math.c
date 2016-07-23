/*
* File name: lib/math.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 常用的数学处理函数
*
*/

#include "math.h"


int pow(int x, int p)
{
	int t = x;
	if (p == 0)
	{
		return 1;
	}
	p--;
	while(p-- && p>-1)
	{
		x *= t;
	}
	return x;
}
int abs(int value)
{
	return value<0?-value:value;
}