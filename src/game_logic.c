#include "game_logic.h"
#include <stdlib.h>
#include "lcg_random.h"

Game* CreateGame(int rows, int cols)
{   Game* game = malloc(sizeof(Game));
    if(game)
    {   game->win_level = 11;
        game->random_max_level = 2;
        game->random_seed = 0;
        game->random = 0;
        game->score = 0;
        game->status = GAME_STATUS_NEW;
        game->board = CreateBoard(4, 4);
        if(game->board == NULL)
        {   free(game);
            game = NULL;
        }
    }
    return game;
}

void DestroyGame(Game* game)
{   if(game == NULL) return;
    DestroyBoard(game->board);
    free(game);
}

void StartGame(Game* game)
{   if(game == NULL) return;
    game->random = game->random_seed;
    if(game->status != GAME_STATUS_NEW)
    {   game->status = GAME_STATUS_NEW;
        game->score = 0;
        ResetBoard(game->board);
    }
}

void NewGame(Game* game)
{   if(game == NULL) return;
    game->random_seed = rand();
    StartGame(game);
}

void NextTurn(Game* game)
{   if(game == NULL) return;
    int cell_count = game->board->rows * game->board->cols;
    int* is = malloc(sizeof(int) * cell_count);
    if(is == NULL)
    {   game->status = GAME_STATUS_ERROR;
        return;
    }
    int n = 0, i;
    for(i = 0; i < cell_count; i++)
    {   if(game->board->cells[i] == 0)
        {   is[n] = i;
            n++;
        }
    }
    if(n > 0)
    {   if(game->status == GAME_STATUS_NEW) game->status = GAME_STATUS_PLAYING;
        game->random = NextRandom(game->random);
        int celli = game->random % n;
        game->random = NextRandom(game->random);
        int cellpow = (game->random % game->random_max_level) + 1;
        game->board->cells[is[celli]] = 1 << cellpow;
    } else if(game->status != GAME_STATUS_WON)
        game->status = GAME_STATUS_LOST;
    free(is);
}

void CheckTurn(Game* game)
{	if(game == NULL) return;
    int cell_count = game->board->rows * game->board->cols;
    int i;
    for(i = 0; i < cell_count; i++)
    {   if(game->board->cells[i] == 0 ||
           (i < cell_count - 1 &&
            game->board->cells[i] == game->board->cells[i + 1]) ||
           (i < cell_count - game->board->cols &&
           game->board->cells[i] == game->board->cells[i + game->board->cols]))
        {   return;
        }
    }
    if(game->status == GAME_STATUS_PLAYING) game->status = GAME_STATUS_LOST;
}

static int PushRowLeft(int* cells, int row, int cols)
{   int begin = row * cols;
    int end = begin + cols;
    int i, j, points = 0;
    for(i = begin; i < end - 1; i++)
    {   if(cells[i])
        {   for(j = i + 1; j < end && ! cells[j]; j++);
            if(j < end && cells[j] == cells[i])
            {   cells[i] *= 2;
                cells[j] = 0;
                points += cells[i];
                i = j + 1;
            }
        }
    }
    for(i = begin; i < end; i++)
    {   for(j = i; j < end && ! cells[j]; j++);
        if(j == end) break;
        if(j > i)
        {   cells[i] = cells[j];
            cells[j] = 0;
            if(points <= 0) points--;
        }
    }
    return points;
}

int PushLeft(Game* game)
{   if(game == NULL) return 0;
    int rows = game->board->rows;
    int cols = game->board->cols;
    int row, hasmoved = 0;
    for(row = 0; row < rows; row++)
    {   int points = PushRowLeft(game->board->cells, row, cols);
        if(points > 0) game->score += points;
        if(!hasmoved && points != 0) hasmoved = 1;
    }
    return hasmoved;
}

static int PushRowRight(int* cells, int row, int cols)
{   int begin = row * cols;
    int end = begin + cols;
    int i, j, points = 0;
    for(i = end - 1; i > begin; i--)
    {   if(cells[i])
        {   for(j = i - 1; j >= begin && ! cells[j]; j--);
            if(j >= begin && cells[j] == cells[i])
            {   cells[i] *= 2;
                cells[j] = 0;
                points += cells[i];
                i = j - 1;
            }
        }
    }
    for(i = end - 1; i >= begin; i--)
    {   for(j = i; j >= begin && ! cells[j]; j--);
        if(j < begin) break;
        if(j < i)
        {   cells[i] = cells[j];
            cells[j] = 0;
            if(points <= 0) points--;
        }
    }
    return points;
}

int PushRight(Game* game)
{   if(game == NULL) return 0;
    int rows = game->board->rows;
    int cols = game->board->cols;
    int row, hasmoved = 0;
    for(row = 0; row < rows; row++)
    {   int points = PushRowRight(game->board->cells, row, cols);
        if(points > 0) game->score += points;
        if(!hasmoved && points != 0) hasmoved = 1;
    }
    return hasmoved;
}

static inline int Index1d(int row, int col, int cols)
{   return row * cols + col;
}

static int PushColumnUp(int* cells, int col, int rows, int cols)
{   int i, j, ii, jj, points = 0;
    for(i = 0; i < rows - 1; i++)
    {   ii = Index1d(i, col, cols);
        if(cells[ii])
        {   for(j = i + 1; j < rows && ! cells[Index1d(j, col, cols)]; j++);
            jj = Index1d(j, col, cols);
            if(j < rows && cells[jj] == cells[ii])
            {   cells[ii] *= 2;
                cells[jj] = 0;
                points += cells[ii];
                i = j + 1;
            }
        }
    }
    for(i = 0; i < rows; i++)
    {   for(j = i; j < rows && ! cells[Index1d(j, col, cols)]; j++);
        if(j == rows) break;
        if(j > i)
        {   cells[Index1d(i, col, cols)] = cells[Index1d(j, col, cols)];
            cells[Index1d(j, col, cols)] = 0;
            if(points <= 0) points--;
        }
    }
    return points;
}

int PushUp(Game* game)
{   if(game == NULL) return 0;
    int rows = game->board->rows;
    int cols = game->board->cols;
    int col, hasmoved = 0;
    for(col = 0; col < cols; col++)
    {   int points = PushColumnUp(game->board->cells, col, rows, cols);
        if(points > 0) game->score += points;
        if(!hasmoved && points != 0) hasmoved = 1;
    }
    return hasmoved;
}

static int PushColumnDown(int* cells, int col, int rows, int cols)
{   int i, j, ii, jj, points = 0;
    for(i = rows - 1; i > 0; i--)
    {   ii = Index1d(i, col, cols);
        if(cells[ii])
        {   for(j = i - 1; j >= 0 && ! cells[Index1d(j, col, cols)]; j--);
            jj = Index1d(j, col, cols);
            if(j >= 0 && cells[jj] == cells[ii])
            {   cells[ii] *= 2;
                cells[jj] = 0;
                points += cells[ii];
                i = j - 1;
            }
        }
    }
    for(i = rows - 1; i >= 0; i--)
    {   for(j = i; j >= 0 && ! cells[Index1d(j, col, cols)]; j--);
        if(j < 0) break;
        if(j < i)
        {   cells[Index1d(i, col, cols)] = cells[Index1d(j, col, cols)];
            cells[Index1d(j, col, cols)] = 0;
            if(points <= 0) points--;
        }
    }
    return points;
}

int PushDown(Game* game)
{   if(game == NULL) return 0;
    int rows = game->board->rows;
    int cols = game->board->cols;
    int col, hasmoved = 0;
    for(col = 0; col < cols; col++)
    {   int points = PushColumnDown(game->board->cells, col, rows, cols);
        if(points > 0) game->score += (points > 0) ? points : 0;
        if(!hasmoved && points != 0) hasmoved = 1;
    }
    return hasmoved;
}
