#include "lib.h"

const char WALL = 'X';
const char PATH = ' ';
const char TAKEN = 'o';
const char ENTER = '+';
const char EXITS = '-';
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

	printf("\n  ");
	for (int j = 0; maze[j]; j++)
	{
		for (int i = 0; maze[j][i]; i++)
		{
			printf("%c ", maze[j][i]);
		}
		printf("\n  ");
	}
	printf("\n");
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

	if (!isPosValid(tmp)) return MBAD;

	mCurr = tmp;
	mMaze[mCurr.y][mCurr.x] = TAKEN;

	if (isEdge(tmp)) return MCOMPLETE;

	return MOK;
}

int isEdge(POINT pos)
{
	if (pos.x == 0 || (pos.y == 0 && pos.x != mSize.x / 2)
	 || pos.x == mSize.x - 1 || pos.y == mSize.y - 1) return 1;
	return 0;
}

int isPosValid(POINT pos)
{
	if (pos.x < 0 || pos.x >= mSize.x || pos.y < 0 || pos.y >= mSize.y
	 || mMaze[pos.y][pos.x] != PATH) return 0;
	return 1;
}

int checkDir(int bDir[])
{
	POINT up, down, left, right;

	up = down = left = right = mCurr;
	up.y--;
	down.y++;
	left.x--;
	right.x++;

	bDir[MUP]    = isPosValid(up);
	bDir[MDOWN]  = isPosValid(down);
	bDir[MLEFT]  = isPosValid(left);
	bDir[MRIGHT] = isPosValid(right);

	return bDir[MUP] + bDir[MDOWN] + bDir[MLEFT] + bDir[MRIGHT];
}

void invertDir(MDIRECTION *dir)
{
	if      (*dir == MUP)    *dir = MDOWN;
	else if (*dir == MDOWN)  *dir = MUP;
	else if (*dir == MLEFT)  *dir = MRIGHT;
	else if (*dir == MRIGHT) *dir = MLEFT;
}

int runSoukup(void)
{
	DIRPOINT stack[BUF80];   // stack of intersection points
	MDIRECTION invDir = MUP; // inverse of the current heading
	MSTATUS status;
	int stackLen = 0, i, idx;
	int bDir[MLEN];
	int pathLen = 1;

	if (!mMaze) return -1;
	mMaze[mCurr.y][mCurr.x] = ENTER;

	for (;; pathLen++)
	{
		//printMaze(NULL);
		if (checkDir(bDir) > 0)
		{
			stack[stackLen].x = mCurr.x;
			stack[stackLen].y = mCurr.y;
			stack[stackLen].qLen = 0;
			for (i = invDir + 1; invDir != i % MLEN; i++)
			{
				if (bDir[i % MLEN] == 0) continue;
				stack[stackLen].dirQ[stack[stackLen].qLen] = i % MLEN;
				stack[stackLen].qLen++;
			}
			stackLen++;
		}
		if (stackLen == 0) return -2;
		idx = stackLen - 1;
		stack[idx].qLen--;
		if (stack[idx].qLen <= 0) stackLen--;
		mCurr.x = stack[idx].x;
		mCurr.y = stack[idx].y;
		invDir = stack[idx].dirQ[stack[idx].qLen];
		status = makeMove(invDir);
		printf("%3d steps, mCurr %2d.%2d, dir %d\n", pathLen, mCurr.x, mCurr.y, invDir);
		if (status == MBAD) return -3;
		if (status == MCOMPLETE)
		{
			mMaze[mCurr.y][mCurr.x] = EXITS;
			return pathLen;
		}
		invertDir(&invDir);
	}
}

