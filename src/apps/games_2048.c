#include "stdio.h"
#include "time.h"
#include "conio.h"
#include "math.h"
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
//Select 2 or 4 randomly.
int selectDigit()
{
	int d;
	d = abs(rand()%(LEN));
	return d%2 == 0? 2:4;
}
void initBoxes(int digit[][LEN])
{
	int temp;
	int posX,  posY;		// The position(X, Y) of digit.
	srand((unsigned)time(NULL));
	posX = abs(rand()%(LEN));	//The random position.
	posY = abs(rand()%(LEN));
	digit[posX][posY] = selectDigit();
	while(1)		//Prevent the position from  being equal to occupied positions.
	{
		if(temp = abs(rand()%(LEN)) != posX)
		{
			posX = temp;
			break;
		}
	}
	while(1)
	{
		if(temp = abs(rand()%(LEN))  != posY)
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
	int i, j;
	int posX,  posY;// the rand position
	srand((unsigned)time(NULL));
	while(1)		//Create a random position.
	{
		posX = abs(rand())%LEN;
		posY = abs(rand())%LEN;
		if (!digit[posX][posY])
			break;
	}
	digit[posX][posY] = selectDigit();
}
int getSpace(int n)		//manage the format.
{
	if(n/10 == 0)//1
		return N-1;
	else if(n/100 == 0)//2
		return N-2;
	else if(n/1000 == 0)//3
		return N-3;
	else		//4
		return N-4;
}

int moveLeft(int digit[][LEN])
{
	int i, j;
	int pos, flag;
	int isMove = 0;		//Judge the positiond if have been moved.
	for(i=0; i<LEN; i++)
	{
		pos = -1;
		flag = 0;
		for(j=0; j<LEN; j++)
		{
			if(!digit[i][j] && !flag)		//The first barrier positon.
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
			if(!digit[j][i] && !flag)		//The first barrier positon
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
			if(!digit[i][j] && !flag)		//The first barrier positon
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
			if(!digit[j][i] && !flag)		//The first barrier positon
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
	int isAdd = 0;	//Judge if there have the same digit.
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
//Judge the game if  will over.
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
	console_clear();		// Clear the screen
	printf("\n                                 2 0 4 8  Game                                   \n");
	printf("================================================================================\n");//82
	for(i=0; i<LEN; i++)
	{
		for(j=0; j<LEN; j++)
		{
			for(k=0; k<N+1; k++)
				printf(" ");
			if(digit[i][j] ==  0)
			{
				for(n=0; n<N; n++)
					printf(" ");
			}
			else
			{
				printf("%d", digit[i][j]);
				for(m=0; m<getSpace(digit[i][j]); m++)
					printf(" ");
			}
			printf(" ");
		}
		printf("\n\n\n");
	}
	printf("================================================================================\n");//82
	for(k=0; k<N+1; k++)
		printf(" ");
	printf("score: %d\n\n", score);
	for(k=0; k<N+1; k++)
		printf(" ");
	printf("Esc---Exit\n\n");
}
void keyDown(int digit[][LEN])
{	int ch, k, m;
	int isMove, isAdd;
	score = 0;
	while(1)
	{
		ch = getch();
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
					printf(" ");
				printf("Exit success~\n\n");
				return ;
		}
		if(isOver(digit))
		{
			for(k=0; k<N+1; k++)
				printf(" ");
			printf("Oh no!Game over~\n\n");
		}
	}
}
int game_play()
{
	int digit[LEN][LEN] = {0};		//The zero means that the position is none.
	initBoxes(digit);			//Init the game.
	keyDown(digit);			//Monitor the keyboard.
	return 0;
}