#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
typedef unsigned long time_t;

void init_timer(uint32_t freq);
void sleep(int ticks);
void srand(int);
int rand();
time_t time(time_t *timer);
#endif
