/*
* File name: apps/games_2048.c
* Author   : 0xC000005
* Version  : 0.1
* Date     : 2016/06/29
* Description: 2048游戏实现
*
*/
#include "../lib/stdio.h"
#include "../lib/math.h"
#include "../lib/timer.h"
#include "../lib/io.h"
#include "../kernel/keyboard.h"
#include "games_2048.h"
#define  LEN 4
#define N 8
#define SPACE 32
#define EXIT 1
#define LEFT 75
#define UP 72
#define RIGHT 77
#define DOWN 80
int score = 0;


int selectDigit()
{
	int d;
	d = abs(rand()%(LEN));
	return d%2 == 0? 2:4;
}
void initBoxes(int digit[][LEN])
{
	int temp;
	int posX,  posY;
	srand((unsigned)time(NULL));
	posX = abs(rand()%(LEN));
	posY = abs(rand()%(LEN));
	digit[posX][posY] = selectDigit();
	while(1)
	{
		if((temp = abs(rand()%(LEN))) != posX)
		{
			posX = temp;
			break;
		}
	}
	while(1)
	{
		if( (temp = abs(rand()%(LEN)))  != posY)
		{
			posY = temp;
			break;
		}
	}
	digit[posX][posY] = selectDigit();
	show(digit);
}
void createDigit(int digit[][LEN])
{
	int posX,  posY;
	srand((unsigned)time(NULL));
	while(1)
	{
		posX = abs(rand())%LEN;
		posY = abs(rand())%LEN;
		if (!digit[posX][posY])
			break;
	}
	digit[posX][posY] = selectDigit();
}
int getSpace(int n)
{
	if(n/10 == 0)
		return N-1;
	else if(n/100 == 0)
		return N-2;
	else if(n/1000 == 0)
		return N-3;
	else
		return N-4;
}

int moveLeft(int digit[][LEN])
{
	int i, j;
	int pos, flag;
	int isMove = 0;
	for(i=0; i<LEN; i++)
	{
		pos = -1;
		flag = 0;
		for(j=0; j<LEN; j++)
		{
			if(!digit[i][j] && !flag)
			{
				pos = j;
				flag = 1;
			}
			if(digit[i][j] && pos >= 0)
			{
				digit[i][pos] = digit[i][j];
				digit[i][j] = 0;
				isMove = 1;

				pos = -1;
				flag = 0;
				j = 0;
			}
		}
	}
	if(isMove)
		return 1;
	return 0;
}
int moveUp(int digit[][LEN])
{
	int i, j;
	int pos, flag;
	int isMove = 0;
	for(i=0; i<LEN; i++)
	{
		pos = -1;
		flag = 0;
		for(j=0; j<LEN; j++)
		{
			if(!digit[j][i] && !flag)
			{
				pos = j;
				flag = 1;
			}
			if(digit[j][i] && pos>=0)
			{
				digit[pos][i] = digit[j][i];
				digit[j][i] = 0;
				isMove = 1;

				pos = -1;
				flag = 0;
				j = 0;
			}
		}
	}
	if(isMove)
		return 1;
	return 0;
}
int moveRight(int digit[][LEN])
{
	int i, j;
	int pos, flag;
	int isMove = 0;
	for(i=0; i<LEN; i++)
	{
		pos = LEN;
		flag = 0;
		for(j=LEN-1; j>=0; j--)
		{
			if(!digit[i][j] && !flag)
			{
				pos = j;
				flag = 1;
			}
			if(digit[i][j] && pos<LEN)
			{
				digit[i][pos] = digit[i][j];
				digit[i][j] = 0;
				isMove = 1;

				flag = 0;
				pos = LEN;
				j = LEN - 1;
			}
		}
	}
	if(isMove)
		return 1;
	return 0;
}
int moveDown(int digit[][LEN])
{
	int i, j;
	int pos, flag;
	int isMove = 0;
	for(i=0; i<LEN; i++)
	{
		pos = LEN;
		flag = 0;
		for(j=LEN-1; j>=0; j--)
		{
			if(!digit[j][i] && !flag)
			{
				pos = j;
				flag = 1;
			}
			if(digit[j][i] && pos<LEN)
			{
				digit[pos][i] = digit[j][i];
				digit[j][i] = 0;
				isMove = 1;

				pos = LEN;
				 flag = 0;
				 j = LEN - 1;
			}
		}
	}
	if(isMove)
		return 1;
	return 0;
}
int addLeft(int digit[][LEN])
{
	int i, j;
	int temp;
	int isAdd = 0;
	for(i=0; i<LEN; i++)
	{
		for(j=0; j<LEN; j++)
		{
			if(j+1<LEN && (temp = digit[i][j] )!= 0 && digit[i][j+1])
			{
				if(temp == digit[i][j+1])
				{
					digit[i][j] = temp + digit[i][j+1];
					digit[i][j+1] = 0;
					isAdd = 1;
					score += temp*2;
				}
			}
		}
	}
	if(isAdd)
		return 1;
	return 0;
}
int addRight(int digit[][LEN])
{
	int i, j;
	int temp;
	int isAdd = 0;
	for(i=0; i<LEN; i++)
	{
		for(j=LEN; j>=0; j--)
		{
			if(j-1>=0 && (temp = digit[i][j] ) != 0 && digit[i][j-1])
			{
				if(temp == digit[i][j-1])
				{
					digit[i][j] = temp + digit[i][j-1];
					digit[i][j-1] = 0;
					isAdd = 1;
					score += temp*2;
				}
			}
		}
	}
	if(isAdd)
		return 1;
	return 0;
}
int addUp(int digit[][LEN])
{
	int i, j;
	int temp;
	int isAdd = 0;
	for(i=0; i<LEN; i++)
	{
		for(j=0; j<LEN; j++)
		{
			if(j+1<LEN && (temp = digit[j][i] ) != 0 && digit[j+1][i])
			{
				if(temp == digit[j+1][i])
				{
					digit[j][i] = temp + digit[j+1][i];
					digit[j+1][i] = 0;
					isAdd = 1;
					score += temp*2;
				}
			}
		}
	}
	if(isAdd)
		return 1;
	return 0;

}
int addDown(int digit[][LEN])
{
	int i, j;
	int temp;
	int isAdd = 0;
	for(i=0; i<LEN; i++)
	{
		for(j=LEN; j>=0; j--)
		{
			if(j-1>=0 && (temp = digit[j][i] ) != 0 && digit[j-1][i])
			{
				if(temp == digit[j-1][i])
				{
					digit[j][i] = temp + digit[j-1][i];
					digit[j-1][i] = 0;
					isAdd = 1;
					score += temp*2;
				}
			}
		}
	}
	if(isAdd)
		return 1;
	return 0;
}

int isOver(int digit[][LEN])
{
	int i, j;
	for(i=0; i<LEN;  i++)
	{
		for(j=0; j<LEN; j++)
		{
			if(!digit[i][j])
				return 0;
		}
	}
	for(i=0; i<LEN;  i++)
	{
		for(j=0; j<LEN; j++)
		{
			if((j+1<LEN && digit[i][j] == digit[i][j+1]) || (i+1<LEN && digit[i][j] == digit[i+1][j]))
				return 0;
		}
	}
	return 1;
}
void show(int digit[][LEN])
{
	int i, j, m, n, k;
	clear_screen();
	kprint("\n                            TinyOs Game 2048                                    \n");
	kprint("--------------------------------------------------------------------------------\n");//82
	for(i=0; i<LEN; i++)
	{
		for(j=0; j<LEN; j++)
		{
			for(k=0; k<N+1; k++)
				kprint(" ");
			if(digit[i][j] ==  0)
			{
				for(n=0; n<N-1; n++)
					kprint(" ");
				kprint("|");
			}
			else
			{
				setTextColor(rc_black, rc_light_red);
				kprint("%d", digit[i][j]);
				setTextColor(rc_black, rc_white);
				for(m=0; m<getSpace(digit[i][j])-1; m++)
					kprint(" ");
				kprint("|");
			}
			kprint(" ");
		}
		kprint("\n");
		kprint("--------------------------------------------------------------------------------\n");
		kprint("\n");
	}

	for(k=0; k<N+1; k++)
		kprint(" ");
	kprint("Score: %d\n\n", score);
	for(k=0; k<N+1; k++)
		kprint(" ");
	kprint("Esc: Exit\n\n");
}
void keyDown(int digit[][LEN])
{	int ch, k;
	int isMove, isAdd;
	score = 0;
	while(1)
	{
		ch = getch();
		kprint("%d",ch);
		switch (ch)
		{
			case LEFT :
				isMove = moveLeft(digit);
				isAdd = addLeft(digit);
				moveLeft(digit);
				if(isMove|| isAdd)
					createDigit(digit);
				show(digit);
				break;
			case RIGHT :
				isMove = moveRight(digit);
				isAdd = addRight(digit);
				moveRight(digit);
				if(isMove|| isAdd)
					createDigit(digit);
				show(digit);
				break;
			case UP:
				isMove = moveUp(digit);
				isAdd = addUp(digit);
				moveUp(digit);
				if(isMove || isAdd)
					createDigit(digit);
				show(digit);
				break;
			case DOWN:
				isMove = moveDown(digit);
				isAdd = addDown(digit);
				moveDown(digit);
				if(isMove || isAdd)
					createDigit(digit);
				show(digit);
				break;
			case EXIT:
				for(k=0; k<N+1; k++)
					kprint(" ");
				kprint("Exit success~\n\n");
				return ;
		}
		if(isOver(digit))
		{
			for(k=0; k<N+1; k++)
				kprint(" ");
			kprint("Oh no!Game over~\n\n");
		}
	}
}
int game_play()
{
	int digit[LEN][LEN] = {{0}};
	initBoxes(digit);
	keyDown(digit);
	return 0;
}