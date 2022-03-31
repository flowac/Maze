#include "lib.h"

int nearbyPaths(char **maze, POINT pos)
{
	int nPaths = 0;
	for (int j = -1; j < 2; j++)
	{
		if (pos.y + j < 0) continue;
		if (!maze[pos.y + j]) break;
		for (int i = -1; i < 2; i++)
		{
			if (maze[pos.y + j][pos.x + i] == PATH)
			{
				nPaths++;
			}
		}
	}

	return nPaths;
}

// Generate a maze based on given dimensions
char **genMaze(int width, int height)
{
	POINT curr, tmp;   // position on the maze
	POINT fifo[BUF80]; // maze filling queue
	int fifoLen = 0;
	char **maze = calloc(height + 1, sizeof(char *));
	for (int j = 0; j < height; j++)
	{
		maze[j] = calloc(width + 1, sizeof(char));
		memset(maze[j], WALL, width);
		maze[j][width] = 0;
	}
	maze[height] = NULL;

	curr.y = 1;
	curr.x = width / 2;
	maze[0][curr.x] = maze[1][curr.x] = PATH;
	while (curr.x > 0 && curr.x < (width - 1) && curr.y > 0 && curr.y < (height - 1))
	{
		if (fifoLen < BUF80)
		{
			fifo[fifoLen++] = curr;
		}

		// this loop might hang if the maze gets stuck
		while (1)
		{
			MDIRECTION dir = (rand() % 4);
			tmp = curr;
			switch (dir)
			{
				case MUP:    tmp.y--; break;
				case MDOWN:  tmp.y++; break;
				case MLEFT:  tmp.x--; break;
				case MRIGHT: tmp.x++; break;
			}
			if ((tmp.x >= 0 && tmp.x < width && tmp.y >= 0 && tmp.y < height)
			 && (nearbyPaths(maze, tmp) < 3)
			 && (maze[tmp.y][tmp.x] == WALL))
			{
				break;
			}
		}
		curr = tmp;
		maze[curr.y][curr.x] = PATH;
	}

	while (fifoLen > 0)
	{
		curr = fifo[0];
		fifoLen--;
		memmove(fifo, fifo + 1, sizeof(POINT) * fifoLen);

		for (int n = 0; n < 20; n++)
		{
			for (int m = 0; m < 10; m++)
			{
				MDIRECTION dir = (rand() % 4);
				tmp = curr;
				switch (dir)
				{
					case MUP:    tmp.y--; break;
					case MDOWN:  tmp.y++; break;
					case MLEFT:  tmp.x--; break;
					case MRIGHT: tmp.x++; break;
				}
				if ((tmp.x > 0 && tmp.x < (width - 1) && tmp.y > 0 && tmp.y < (height - 1))
				 && (nearbyPaths(maze, tmp) < 3)
				 && (maze[tmp.y][tmp.x] == WALL))
				{
					curr = tmp;
					maze[curr.y][curr.x] = PATH;
					if (fifoLen < BUF80)
					{
						fifo[fifoLen++] = curr;
					}
					break;
				}
			}
		}
	}
	return maze;
}

int main(int argc, char** argv)
{
	srand(time(NULL));

	char **maze = genMaze(15, 15);
	printMaze(maze);

	mMaze = maze;
	mSize.x = 15;
	mSize.y = 15;
	mCurr.x = mSize.x / 2;
	mCurr.y = 0;
	printf("\nSteps taken: %d\n", runSoukup());
	printMaze(maze);

	freeMaze(maze);

	return 0;
}

