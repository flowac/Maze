#include "lib.h"

#define BUF80 80

const char WALL = 'X';
const char PATH = ' ';
POINT mSize; // max width and length (starts counting from 1)
POINT mCurr; // current maze position (starts counting from 0)

char **mMaze = NULL;

void freeMaze(char **maze)
{
	char **mPtr = maze;
	if (!mPtr) mPtr = mMaze;
	if (!mPtr) return;

	for (int i = 0; mPtr[i]; i++)
	{
		free(mPtr[i]);
	}
	free(mPtr);
}

void printMaze(char **maze)
{
	char **mPtr = maze;
	if (!mPtr) mPtr = mMaze;
	if (!mPtr) return;

	printf("\n\n");
	for (int j = 0; maze[j]; j++)
	{
		for (int i = 0; maze[j][i]; i++)
		{
			printf("%c ", maze[j][i]);
		}
		printf("\n");
	}
}

int initMaze(char *configPath)
{
	char buf[BUF80], *endPtr, *tmpPtr, **newMaze;
	int tmp;
	FILE *cfg = fopen(configPath, "r");

	if (!cfg) return 1;
	fgets(buf, BUF80, cfg);
	errno = 0;
	tmp = strtol(buf, &endPtr, 0);
	if (errno != 0) return 2;
	mSize.x = tmp;

	tmpPtr = endPtr + 1;
	errno = 0;
	tmp = strtol(tmpPtr, &endPtr, 0);
	if (errno != 0) return 3;
	mSize.y = tmp;

	mCurr.x = mSize.x / 2;
	mCurr.y = 0;

	newMaze = calloc(mSize.y + 1, sizeof(char *));
	for (int j = 0; j < mSize.y && fgets(buf, BUF80, cfg); j++)
	{
		newMaze[j] = calloc(mSize.x + 1, sizeof(char));
		memcpy(newMaze[j], buf, mSize.x);
		newMaze[j][mSize.x] = 0;
	}
	newMaze[mSize.y] = NULL;
	fclose(cfg);

	return 0;
}

MSTATUS makeMove(MDIRECTION dir)
{
	POINT tmp = mCurr;
	switch (dir)
	{
		case MUP:    tmp.y--; break;
		case MDOWN:  tmp.y++; break;
		case MLEFT:  tmp.x--; break;
		case MRIGHT: tmp.x++; break;
	}

	if (tmp.x < 0 || tmp.x >= mSize.x || tmp.y < 0 || tmp.y >= mSize.y) return MBAD;

	if (mMaze[tmp.y][tmp.x] == WALL) return MBAD;

	if ((tmp.x == 0 || tmp.x == mSize.x - 1 || tmp.y == 0 || tmp.y == mSize.y - 1)
	 && !((tmp.x == mSize.x / 2) && tmp.y == 0)) return MCOMPLETE;

	return MOK;
}

