/*
* File name: lib/timer.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 定时器
*
*/

#include "../kernel/isr.h"
#include "../kernel/ports.h"
#include "../kernel/system.h"
#include "timer.h"
#include "function.h"

#ifndef NULL
    #define NULL 0
#endif

uint32_t tick = 0;
volatile unsigned long int _thours = 00;
volatile unsigned long int _tminutes = 00;
volatile unsigned long int _tseconds = 00;

static void timer_callback(registers_t *regs) {
    tick++;
    if (tick % 100 == 0)
    {
        _tseconds++;

        if( _tseconds == 60 )
        {
            _tminutes++;
            _tseconds = 00;

            if(_thours == 60)
            {
               _thours++;
               _tminutes = 00;
            }
        }
    }
    UNUSED(regs);
}

void init_timer(uint32_t freq) {
    register_interrupt_handler(IRQ0, timer_callback);

    uint32_t divisor = 1193180 / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}

void sleep(int ticks)
{
    unsigned long eticks;
    time_t t1 = time(NULL);
    eticks = t1 + ticks;
    while(t1 < eticks)
      t1 = time(NULL);
}

static long days_between_dates(unsigned start_year, unsigned start_day,
    int end_year, unsigned end_day)
{
  int fourcents, centuries, fouryears, years;
  long days;

  fourcents = end_year / 400 - start_year / 400;

  centuries = end_year / 100 - start_year / 100 - fourcents * 4;

  fouryears = end_year / 4 - start_year / 4 - fourcents * 100 - centuries * 25;

  years = end_year - start_year - 400 * fourcents - 100 * centuries - 4 * fouryears;

  days = (365L * 400 + 97) * fourcents;

  days += (365L * 100 + 24) * centuries;

  days += (365L * 4 + 1) * fouryears;

  days += (365L * 1) * years;

  days += ((long)end_day - start_day);

  if(end_year % 4 == 0 && end_day >= 60)
  {
    days++;
    if(end_year % 100 == 0)
      days--;
    if(end_year % 400 == 0)
      days++;
  }
  if(end_year >= 0)
  {
    days++;
    if(end_year % 4 == 0)
      days--;
    if(end_year % 100 == 0)
      days++;
    if(end_year % 400 == 0)
      days--;
  }
  if(start_year > 0)
    days--;
  return days;
}


#define EPOCH_YEAR  1970
#define EPOCH_DAY 0

static unsigned long date_time_to_time_t(unsigned year, unsigned month,
    unsigned date, unsigned hour, unsigned min, unsigned sec)
{
  static const unsigned days_to_date[12] =
  {
    0,
    31,
    31 + 28,
    31 + 28 + 31,
    31 + 28 + 31 + 30,
    31 + 28 + 31 + 30 + 31,
    31 + 28 + 31 + 30 + 31 + 30,
    31 + 28 + 31 + 30 + 31 + 30 + 31,
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30
  };

  unsigned long rv;
  unsigned day;


  if(month < 1 || month > 12 || date < 1 || date > 31)
    return 0;
  month--;
  date--;
  day = date + days_to_date[month];

  rv = days_between_dates(EPOCH_YEAR, EPOCH_DAY, year, day);

  rv *= 24;
  rv += hour;
  rv *= 60;
  rv += min;
  rv *= 60;
  rv += sec;
  return rv;
}


time_t time(time_t *timer)
{
  static signed char bcd = -1;

  unsigned portval, timeout, month, date, hour, minute, second;
  time_t rv;
  int year;

  if(bcd == -1)
  {
    port_byte_out(0x70, 0x0B);
    if(port_byte_in(0x71) & 0x04)
      bcd = 0;
    else
      bcd = 1;
  }
  while(1)
  {
    port_byte_out(0x70, 0x0A);
    if((port_byte_in(0x71) & 0x80) == 0)
      break;
    port_byte_out(0x70, 0x0B);
    portval = port_byte_in(0x71);
    port_byte_out(0x71, portval | 0x10);
    timeout = 5;
    sleep(timeout);
    port_byte_out(0x71, portval & ~0x10);
  }

  year = read_cmos(9, bcd);
  if(year < 70)
    year += 2000;
  else
    year += 1900;
  month = read_cmos(8, bcd);
  date = read_cmos(7, bcd);
  /* get time */
  hour = read_cmos(4, bcd);
  minute = read_cmos(2, bcd);
  second = read_cmos(0, bcd);

  rv = date_time_to_time_t(year, month, date, hour, minute, second);
  if(timer != NULL)
    (*timer) = rv;
  return rv;
}


static int g_seed;


void srand(int new_seed)
{
  g_seed = new_seed;
}


int rand()
{
  if(g_seed == 0)
    g_seed = 1;
  if((((g_seed << 3) ^ g_seed) & 0x80000000uL) != 0)
    g_seed = (g_seed << 1) | 1;
  else
    g_seed <<= 1;
  return g_seed - 1;
}
