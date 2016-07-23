#ifndef _STDIO_H
#define _STDIO_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define NULL 0

#ifndef SCANF_MAX_BUFFER_LENGTH
#define SCANF_MAX_BUFFER_LENGTH 4096
#endif

#ifndef RUN_SUCCESS
#define RUN_SUCCESS 0
#endif


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


/* Public kernel API */
void clear_screen();
void kprint_at(char *message, int col, int row);
void kprint(const char *format , ...);
void putch(char c);
/*
Now support %s %c %d
*/
void kprint_backspace();

int scanf(const char *format, ...);

void gets(char *chs);

void onKeyDown(char keycode);

void onKeyUp(char keycode);

void registerListenKey(void (*function)(char keycode));

void setTextColor(real_color_t back, real_color_t front);

#endif
