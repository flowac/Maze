#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF80 80

typedef enum
{
	MOK       = 0, // move registered
	MBAD      = 1, // cannot move because of a wall
	MCOMPLETE = 2  // exit reached
} MSTATUS;

typedef enum
{
	MUP    = 0,
	MDOWN  = 1,
	MLEFT  = 2,
	MRIGHT = 3
} MDIRECTION;

typedef struct
{
	int x, y;
} POINT;

extern const char WALL;
extern const char PATH;

void    freeMaze(char **maze);      // initMaze must be called before this, frees global if arg is NULL
void    printMaze(char **maze);     // prints the global maze if arg is NULL
int     initMaze(char *configPath); // return 0 for success
MSTATUS makeMove(MDIRECTION dir);

