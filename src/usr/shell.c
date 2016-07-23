/*
* File name: usr/shell.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 命令终端
*
*/
#include "../lib/stdio.h"
#include "../lib/string.h"
#include "../kernel/system.h"
#include "../lib/timer.h"
#include "../lib/math.h"
#include "../apps/games_2048.h"
#include "shell.h"

#define SHELL_MAX_LENGTH 1024

void dealShell(char* shell)
{
	int result = 0;

	char x[100][100];
	split(shell, ' ',x);
	kprint(shell);
	if (!strcmp(shell, "reboot"))
	{
		reboot();
	}
	else if (!strcmp(shell, "2048"))
	{
		result = game_play();
	}
	else if (!strcmp(shell, "rand"))
	{
		kprint("%d\n", abs(rand()));
	}
	else if (!strcmp(shell, "clear"))
	{
		clear_screen();
	}
	else if (!strcmp(shell, "help"))
	{
		kprint("Usage 'commands [option]...' to call commands.\n");
		kprint("TinyOs support the follows commands:\n");
		kprint("reboot    ------ Reboot system.\n");
		kprint("clear     ------ Clear  screen.\n");
		kprint("2048      ------ Play   2048 game.\n");
		kprint("rand      ------ Create a rand number.\n");
		kprint("help      ------ Show   commands .\n");
	}
	else if (!strcmp(shell, ""))
	{
	}
	else
	{
		kprint("Command not found, Type `help' to see commands list\n");
	}
	if (result)
	{
		kprint("Error, the %s return code(%d)!\n", shell, result);
	}
}

int shell()
{
	char sh[SHELL_MAX_LENGTH];
	srand(time(NULL));
	clear_screen();
	kprint("---------------------------------Welcome to TinyOs !---------------------------\n");
	kprint("\n");
	while(1)
	{
		setTextColor(rc_black, rc_green);
		kprint("TinyOs >");
		setTextColor(rc_black, rc_white);
		kprint(" ");
		gets(sh);
		dealShell(sh);
	}
	return RUN_SUCCESS;
}

