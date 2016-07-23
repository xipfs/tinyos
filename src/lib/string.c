/*
* File name: lib/string.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 字符串处理
*
*/

#include "string.h"
#include "math.h"
#include <stdint.h>


void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void hex_to_ascii(int n, char str[]) {
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    int32_t tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void strcpy(char* to, char* from)
{
    char *to1 = to;
    while(*from)
    {
        *to1++ = *from++;
    }
    *to1++ = '\0';
}

int convertStringToInt(char *str)
{
    int t = 0, p = 0;
    int len;
    len = strlen(str);
    while(len)
    {
        t+= (str[len-1]-'0')*pow(10, p++);
        len--;
    }
    return t;
}

void split(char *str, char ch,char x[][100])
{
    int len = strlen(str);
    int i = 0;
    int j = 0;
    for (i = 0; i<len; i++)
    {
        if (str[i] == ch)
        {
            str[i] = '\0';
        }
    }
    for (i = 0; i<len; )
    {
        while(!str[i++]);
        strcpy(x[j++], str+i-1);
        while(str[i++]);
    }
    x[j][0] = '\0';
}