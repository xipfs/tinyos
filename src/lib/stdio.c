#include "stdio.h"
#include "keyboard_map.h"
#include "../cpu/ports.h"

static uint16_t *video_memory = (uint16_t *)0xB8000;



static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static uint8_t is_shift_down = 0;
static uint8_t font_size = 80;


static real_color_t now_set_color_back = rc_black;
static real_color_t now_set_color_front = rc_white;



static void move_cursor()
{

    uint16_t cursorLocation = cursor_y * font_size + cursor_x;


    outb(0x3D4, 14);                    // 告诉 VGA 我们要设置光标的高字节
    outb(0x3D5, cursorLocation >> 8);   // 发送高 8 位
    outb(0x3D4, 15);                    // 告诉 VGA 我们要设置光标的低字节
    outb(0x3D5, cursorLocation);        // 发送低 8 位
}

//Clear the screen.
void console_clear()
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    int i;
    for (i = 0; i < font_size * 25; i++)
    {
        video_memory[i] = blank;    //Full the line with blank.
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();  //Move cursor.
}


//Move screen up
static void scroll()
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);  // space 是 0x20

    // cursor_y 到 25 的时候，就该换行了
    if (cursor_y >= 25)
    {
        // 将所有行的显示数据复制到上一行，第一行永远消失了...
        int i;

        for (i = 0 * font_size; i < 24 * font_size; i++)   //Full the i-th line with i+1-th line.
        {
            video_memory[i] = video_memory[i+80];
        }

        // 最后的一行数据现在填充空格，不显示任何字符
        for (i = 24 * font_size; i < 25 * font_size; i++)
        {
            video_memory[i] = blank;
        }

        // 向上移动了一行，所以 cursor_y 现在是 24
        cursor_y = 24;
    }
}

//Put a char to the screen with given color.
void putch_color(char c, real_color_t back, real_color_t fore)
{
    uint8_t back_color = (uint8_t)back;
    uint8_t fore_color = (uint8_t)fore;

    uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
    uint16_t attribute = attribute_byte << 8;

    if (c == 0x08 && cursor_x)      //Backspace
    {
        cursor_x--;
    }
    else if (c == 0x09)     //Tab
    {
        cursor_x = (cursor_x+8) & ~(8-1);
    }
    else if (c == '\r')
    {
        cursor_x = 0;
    }
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if (c >= ' ')  //If the char is a normal character, show it.
    {
        video_memory[cursor_y*80 + cursor_x] = c | attribute;
        cursor_x++;
    }

    if (cursor_x >= 80)     //Set cursor.
    {
        cursor_x = 0;
        cursor_y ++;
    }

    scroll();       //Scroll screen if need.
    move_cursor();
}

//Putch a char at given pos and color.
void putch_color_pos(char c, real_color_t back, real_color_t fore, int x, int y)
{
    cursor_x = x;       //Set cursor.
    cursor_y = y;
    putch_color(c, back, fore);
}

//Write a string to screen.
void console_write(char *cstr)
{
    while (*cstr)
    {
        putch(*cstr++);
    }
}

//Put a char to the screen.
void putch(char c)
{
    putch_color(c, now_set_color_back, now_set_color_front);        //Call putch_color with setted color.
}

//Write a unsigned int var to the screen.
void console_write_uint32_t(uint32_t t)
{

    uint32_t tmp = 1000000000;
    uint32_t tmp1;
    char isFind = 0;
    while(tmp)
    {
        tmp1 = t/tmp;
        t = t%tmp;
        if (tmp1)
        {
            putch('0'+tmp1);
            isFind = 1;
        }
        else
        {
            if (isFind)
            {
                putch('0');
            }
        }
        tmp = tmp/10;
    }

}

void console_write_uint8_t(uint8_t t)
{

    uint8_t tmp = 100;
    uint8_t tmp1;
    char isFind = 0;
    while(tmp)
    {
        tmp1 = t/tmp;
        t = t%tmp;
        if (tmp1)
        {
            putch('0'+tmp1);
            isFind = 1;
        }
        else
        {
            if (isFind)
            {
                putch('0');
            }
        }
        tmp = tmp/10;
    }

}

//Write a string with given color.
void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
    while (*cstr)   //Call putch_color to print string.
    {
        putch_color(*cstr++, back, fore);
    }
}

//Deal print
void itoa (char *buf, int base, int d)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    /* If %d is specified and D is minus, put `-' in the head. */
    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;

    /* Divide UD by DIVISOR until UD == 0. */
    do
    {
        int remainder = ud % divisor;

        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);

    /* Terminate BUF. */
    *p = 0;

    /* Reverse BUF. */
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

//The printf function.
//It support %s %d %x
void printf (const char *format, ...)
{
    char **arg = (char **) &format;     //Get the args.
    int c;
    char buf[20];

    arg++;

    while ((c = *format++) != 0)        //Exec format.
    {
        if (c != '%')   //If now %, print it
            putch(c);
        else
        {
            char *p;

            c = *format++;  //Get the next arg.
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

            default:        //If not support it, print the char.
                putch (*((int *) arg++));
                break;
            }
        }
    }
}

//Get a character by keycode.
char getCharByKeyCode(char keycode)
{
    char ch;
    if (keycode == 42 || keycode == 54)     //Shift is down.
    {
        is_shift_down = 1;
    }
    ch = keyboard_map[keycode];
    if (is_shift_down)
    {
        if (ch>='a' && ch <= 'z')       //Switch lowwer case and upper case.
        {
            ch += 'A' - 'a';
        }
        else if (ch>='A' && ch<='Z')
        {
            ch += 'a' - 'A';
        }
        else
        {
            ch = keyboard_map_1[keycode];
        }
    }
    return ch;
}

//Get a string while Enter pressed.
void gets(char *chs)
{
    char ch;
    int pos = 0;
    int keycode;
    do
    {
        keycode = getch();  //By getch() get the keycode.
        ch = getCharByKeyCode(keycode);
        keycode = -1;
        if (ch == '\b')     //If backspace pressed.
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
    while(ch!='\n');
    chs[pos-1] = '\0';
}

//The scanf function, to get user input.
int scanf(const char *format, ...)
{
    char ch;
    int pos = 0;
    int pos1 = 0;
    int keycode;
    char *p;
    char **arg = (char **) &format;
    char scanfStr[SCANF_MAX_BUFFER_LENGTH];
    arg++;
    gets(scanfStr);     //By gets(), get a line.
    pos = 0;
    while(*format)      //Decode the line to each var.
    {
        if (*format == '%')
        {

            char tmp[SCANF_MAX_BUFFER_LENGTH];
            ch = *++format;
            pos1 = 0;

            p = *arg++;
            while(scanfStr[pos] == ' ') pos++;  //Clear space
            switch(ch)
            {
            case 'd':
                while(scanfStr[pos] != ' ' && scanfStr[pos])
                {
                    tmp[pos1++] = scanfStr[pos++];
                }
                tmp[pos1++] = '\0';
                *(int*)p = convertStringToInt(tmp);     //Convert string to a integer.
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

//The var of the callback function.
static void (*onGetKeyFunction)(char keycode);

//The flag that is send a key.
static int isSendKeyCode = 0;

//Set the callback function.
void registerListenKey(void (*function)(char keycode))
{
    onGetKeyFunction = function;
    isSendKeyCode = 0;
}

void onKeyDown(char keycode)
{
    // printf("%c", ch);
    if (onGetKeyFunction && !isSendKeyCode)
    {
        (*onGetKeyFunction)(keycode); //Call
        isSendKeyCode = 1;
    }
    // putch(ch);
}

void onKeyUp(char keycode)
{
    switch(keycode)
    {
        case 42:
        case 54:    //Shift up envent.
            is_shift_down = 0;
            break;
    }
}


static char keycode = -1;


int getch()
{
    char keycode1;
    // printf("onGetKeyFunction:%d\n", &onGetKeyFunction);
    registerListenKey(&onGetKeyFunction);       //Register the callback function.
    while(keycode == -1);       //Wait for user press key.
    keycode1 = keycode;
    keycode = -1;
    return keycode1;
}
int getchInStep(uint32_t step)
{
    char keycode1;
    // printf("onGetKeyFunction:%d\n", &onGetKeyFunction);
    registerListenKey(&onGetKeyFunction);
    while(step--);       //Wait for user press key in steps.
    keycode1 = keycode;
    keycode = -1;
    return keycode1;
}

