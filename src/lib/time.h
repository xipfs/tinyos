#ifndef _TIME_H
#define _TIME_H 
#include "stdio.h"

int rang();
void srand(int);
void timer_install();
void sleep(int ticks);
time_t time(time_t *timer);

#endif