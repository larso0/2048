#ifndef _2048_BOARD_H
#define _2048_BOARD_H
#endif

#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    uint32_t rows, cols;
    uint32_t* cells;
} Board;

/*
 * Creates an empty board of size rows*cols.
 * Returns a nullpointer if there was an error.
 * Errors:
 * Either rows or cols is 0
 * Failure to allocate memory
 */
Board* CreateBoard(uint32_t rows, uint32_t cols);

/*
 * Frees the memory occupied by a board.
 */
void DestroyBoard(Board* board);

/*
 * Resets all the cells on a board to 0.
 */
void ResetBoard(Board* board);
