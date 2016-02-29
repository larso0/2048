#ifndef _2048_BOARD_H
#define _2048_BOARD_H
#endif

#include <stdlib.h>
#include <stdint.h>

typedef struct
{   int rows, cols;
    int* cells;
} Board;

/*
 * Creates an empty board of size rows*cols.
 * Returns a nullpointer if there was an error.
 * Errors:
 * Either rows or cols is 0
 * Failure to allocate memory
 */
Board* CreateBoard(int rows, int cols);

/*
 * Frees the memory occupied by a board.
 */
void DestroyBoard(Board* board);

/*
 * Resets all the cells on a board to 0.
 */
void ResetBoard(Board* board);
