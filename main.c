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
	POINT curr, tmp; // position on the maze
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
		while (1)
		{
			MDIRECTION dir = rand() % 4;
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

	return maze;
}

int main(int argc, char** argv)
{
	srand(time(NULL));

	char **maze = genMaze(15, 15);
	printMaze(maze);
	freeMaze(maze);

	return 0;
}

