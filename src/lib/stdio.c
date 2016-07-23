/*
* File name: lib/stdio.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 标准输入输出
*
*/
#include "stdio.h"
#include "mem.h"
#include "string.h"
#include "io.h"
#include "../kernel/ports.h"
#include <stdint.h>

const char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
  '9', '0', '-', '=', '\b', /* Backspace */
  '\t',     /* Tab */
  'q', 'w', 'e', 'r', /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,      /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
 '\'', '`',   0,    /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',      /* 49 */
  'm', ',', '.', '/',   0,        /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

//The map of the keycode and character when shift button pressed.
const char keyboard_map_1[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
  '(', ')', '_', '+', '\b', /* Backspace */
  '\t',     /* Tab */
  'q', 'w', 'e', 'r', /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', /* Enter key */
    0,      /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', /* 39 */
 '"', '~',   0,    /* Left shift */
 '|', 'z', 'x', 'c', 'v', 'b', 'n',      /* 49 */
  'm', '<', '>', '?',   0,        /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};


static real_color_t now_set_color_back = rc_black;
static real_color_t now_set_color_front = rc_white;


int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

void kprint_at(char *message, int col, int row) {
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void itoa (char *buf, int base, int d)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;

    do
    {
        int remainder = ud % divisor;

        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);


    *p = 0;

    p1 = buf;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void kprint(const char *format , ...){
    char **arg = (char **) &format;
    int c;
    char buf[20];

    arg++;

    while ((c = *format++) != 0)
    {
        if (c != '%')
            putch(c);
        else
        {
            char *p;

            c = *format++;
            switch (c)
            {
            case 'd':
            case 'x':
                itoa (buf, c, *((int *) arg++));
                p = buf;
                while (*p)
                    putch (*p++);
                break;

            case 's':
                p = *arg++;
                if (! p)
                    p = "(null)";

                while (*p)
                    putch (*p++);
                break;

            default:
                putch (*((int *) arg++));
                break;
            }
        }
    }
}

void kprint_backspace() {
    int offset = get_cursor_offset()-2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(0x08, col, row, WHITE_ON_BLACK);
}


void putch(char c){
    int offset = get_cursor_offset();
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);

    uint8_t back_color = (uint8_t)now_set_color_back;
    uint8_t fore_color = (uint8_t)now_set_color_front;

    uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);

    print_char(c, col, row, attribute_byte);
}


int print_char(char c, int col, int row, char attr) {
    uint8_t *vidmem = (uint8_t*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else if (c == 0x08) {
        offset = get_cursor_offset()-2;
        vidmem[offset] = ' ';
        vidmem[offset+1] = attr;
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++)
            memory_copy((uint8_t*)(get_offset(0, i) + VIDEO_ADDRESS),
                        (uint8_t*)(get_offset(0, i-1) + VIDEO_ADDRESS),
                        MAX_COLS * 2);

        char *last_line = (char*) (get_offset(0, MAX_ROWS-1) + (uint8_t*) VIDEO_ADDRESS);
        for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset() {

    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    uint8_t *screen = (uint8_t*) VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }


#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
static uint8_t is_shift_down = 0;

char getCharByKeyCode(char keycode)
{
    char ch;
    if (keycode == 42 || keycode == 54)
    {
        is_shift_down = 1;
    }
    ch = keyboard_map[(int)keycode];

    if (is_shift_down)
    {
        if (ch>='a' && ch <= 'z')
        {
            ch += 'A' - 'a';
        }
        else if (ch>='A' && ch<='Z')
        {
            ch += 'a' - 'A';
        }
        else
        {
            ch = keyboard_map_1[(int)keycode];
        }
    }
    return ch;
}

void gets(char *chs)
{
    char ch;
    int pos = 0;
    int keycode;
    do
    {
        keycode = getch();
        ch = getCharByKeyCode(keycode);
        keycode = -1;
        if (ch == '\b')
        {
            if (pos>0)
            {
                pos--;
                putch('\b');
                putch(' ');
                putch('\b');
            }
        }
        else
        {
            if (ch && ch != (char)27)
            {
                chs[pos++] = ch;
                putch(ch);
            }
        }
    }
    while(ch!= '\n');
    chs[pos-1] = '\0';
}

int scanf(const char *format, ...)
{
    char ch;
    int pos = 0;
    int pos1 = 0;
    char *p;
    char **arg = (char **) &format;
    char scanfStr[SCANF_MAX_BUFFER_LENGTH];
    arg++;
    gets(scanfStr);
    pos = 0;
    while(*format)
    {
        if (*format == '%')
        {

            char tmp[SCANF_MAX_BUFFER_LENGTH];
            ch = *++format;
            pos1 = 0;

            p = *arg++;
            while(scanfStr[pos] == ' ') pos++;
            switch(ch)
            {
            case 'd':
                while(scanfStr[pos] != ' ' && scanfStr[pos])
                {
                    tmp[pos1++] = scanfStr[pos++];
                }
                tmp[pos1++] = '\0';
                *(int*)p = convertStringToInt(tmp);
                break;
            case 'c':
                *p = scanfStr[pos++];
                break;
            case 's':
                while(scanfStr[pos] != ' ' && scanfStr[pos])
                {
                    tmp[pos1++] = scanfStr[pos++];
                }
                tmp[pos1++] = '\0';
                strcpy(p, tmp);
                break;
            }
        }
        format++;
    }
    return 0;
}

void setTextColor(real_color_t back, real_color_t front)
{
    now_set_color_back = back;
    now_set_color_front = front;
}

static void (*onGetKeyFunction)(char keycode);
static int isSendKeyCode = 0;

void registerListenKey(void (*function)(char keycode))
{
    onGetKeyFunction = function;
    isSendKeyCode = 0;
}

void onKeyDown(char keycode)
{

    if (onGetKeyFunction && !isSendKeyCode)
    {
        (*onGetKeyFunction)(keycode); //函数调用
        isSendKeyCode = 1;
    }

}

void onKeyUp(char keycode)
{
    switch(keycode)
    {
        case 42:
        case 54:
            is_shift_down = 0;
            break;
    }
}
