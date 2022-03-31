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
	MRIGHT = 0,
	MUP    = 1,
	MLEFT  = 2,
	MDOWN  = 3,
	MLEN   = 4,
	MNONE  = 5
} MDIRECTION;

typedef struct
{
	int x, y;
} POINT;

typedef struct
{
	int x, y, qLen;
	MDIRECTION dirQ[MLEN - 1];
} DIRPOINT; // a point with a queue of potential directions

extern const char WALL;
extern const char PATH;
extern const char TAKEN;
extern const char RETRACED;

extern char **mMaze;
extern POINT mSize;
extern POINT mCurr;

void    freeMaze(char **maze);      // frees  global maze if arg is NULL
void    printMaze(char **maze);     // prints global maze if arg is NULL
int     initMaze(char *configPath); // sets   global maze, return 0 for success
MSTATUS makeMove(MDIRECTION dir);

int     isEdge(POINT pos);          // returns 1 if position is on the edge
int     isPosValid(POINT pos);      // returns 0 if position is out of range in the maze
int     checkDir(int *dirs);        // returns the count of potential directions
int     runSoukup(void);            // returns (+)length of maze path or (-)error code

