#include "board.h"

Board* CreateBoard(int rows, int cols)
{   if(rows == 0 || cols == 0) return NULL;
    Board* board = malloc(sizeof(Board));
    if(board)
    {   int cellc = rows*cols;
        board->cells = malloc(sizeof(int)*cellc);
        if(board->cells == NULL)
        {   free(board);
            return NULL;
        }
        board->rows = rows;
        board->cols = cols;
        int i;
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
    {   int n = board->rows * board->cols, i;
        for(i = 0; i < n; i++) board->cells[i] = 0;
    }
}
