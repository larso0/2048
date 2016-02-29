#include "board.h"

Board* CreateBoard(uint32_t rows, uint32_t cols)
{   if(rows == 0 || cols == 0) return NULL;
    Board* board = malloc(sizeof(Board));
    if(board)
    {   uint32_t cellc = rows*cols;
        board->cells = malloc(sizeof(uint32_t)*cellc);
        if(board->cells == NULL)
        {   free(board);
            return NULL;
        }
        board->rows = rows;
        board->cols = cols;
        uint32_t i;
        for(i = 0; i < cellc; i++) board->cells[i] = 0;
    }
    return board;
}

void DestroyBoard(Board* board)
{   if(board)
    {   free(board->cells);
        free(board);
    }
}

void ResetBoard(Board* board)
{   if(board)
    {   uint32_t n = board->rows * board->cols, i;
        for(i = 0; i < n; i++) board->cells[i] = 0;
    }
}
