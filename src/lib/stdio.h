#ifndef _STDIO_H
#define _STDIO_H


#ifndef NULL
    #define NULL 0
#endif

#ifndef SCANF_MAX_BUFFER_LENGTH
#define SCANF_MAX_BUFFER_LENGTH 4096
#endif

#ifndef RUN_SUCCESS
#define RUN_SUCCESS 0
#endif

#include<stdint.h>


//Color
typedef
enum real_color {
    rc_black = 0,
    rc_blue = 1,
    rc_green = 2,
    rc_cyan = 3,
    rc_red = 4,
    rc_magenta = 5,
    rc_brown = 6,
    rc_light_grey = 7,
    rc_dark_grey = 8,
    rc_light_blue = 9,
    rc_light_green = 10,
    rc_light_cyan = 11,
    rc_light_red = 12,
    rc_light_magenta = 13,
    rc_light_brown  = 14,   // yellow
    rc_white = 15
} real_color_t;

// 清屏操作
void console_clear();
void putch_color_pos(char c, real_color_t back, real_color_t fore, int x, int y);
void printf(const char *format , ...);
int scanf(const char *format, ...);
void gets(char *chs);
void onKeyDown(char keycode);
void onKeyUp(char keycode);
void registerListenKey(void (*function)(char keycode));
void setTextColor(real_color_t back, real_color_t front);
int getch();
int getchInStep(uint32_t step);
#endif
