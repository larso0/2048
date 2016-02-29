#include "game_logic.h"
#include <stdlib.h>
#include "lcg_random.h"
#include <math.h>

Game* CreateGame(uint32_t rows, uint32_t cols, uint32_t max_cell, uint32_t max_random)
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
    uint32_t cell_count = game->board->rows * game->board->cols;
    uint32_t* is = malloc(sizeof(uint32_t) * cell_count);
    if(is == NULL)
    {   game->status = GAME_STATUS_ERROR;
        return;
    }
    uint32_t n = 0, i;
    for(i = 0; i < cell_count; i++)
    {   if(game->board->cells[i] == 0)
        {   is[n] = i;
            n++;
        }
    }
    if(n > 0)
    {   if(game->status == GAME_STATUS_NEW) game->status = GAME_STATUS_PLAYING;
        game->random = NextRandom(game->random);
        uint32_t celli = game->random % n;
        game->random = NextRandom(game->random);
        uint32_t cellpow = (game->random % game->random_max_level) + 1;
        game->board->cells[is[celli]] = 1 << cellpow;
    } else if(game->status != GAME_STATUS_WON)
        game->status = GAME_STATUS_LOST;
    free(is);
}

static inline void PushRowLeft(uint32_t* cells, uint32_t begin, uint32_t end)
{   size_t i;
    for(i = begin; i < end - 1; i++)
    {   if(cells[i])
        {   uint32_t j;
            for(j = i + 1; j < end && ! cells[j]; j++);
            if(cells[j] == cells[i])
            {   cells[i] *= 2;
                cells[j] = 0;
                i = j + 1;
            }
        }
    }
    for(i = begin; i < end; i++)
    {   uint32_t j;
        for(j = i; j < end && ! cells[j]; j++);
        if(j < end && j > i)
        {   cells[i] = cells[j];
            cells[j] = 0;
        } else break;
    }
}

void PushLeft(Game* game)
{   if(game == NULL) return;
    uint32_t rows = game->board->rows;
    uint32_t cols = game->board->cols;
    uint32_t row;
    for(row = 0; row < rows; row++)
        PushRowLeft(game->board->cells, row * cols, row * cols + cols);
}
