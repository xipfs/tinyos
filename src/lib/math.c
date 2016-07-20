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