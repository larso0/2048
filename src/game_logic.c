#include "game_logic.h"
#include <stdlib.h>
#include "lcg_random.h"

Game* CreateGame(uint32_t rows, uint32_t cols, uint32_t max_cell, uint32_t max_random)
{
    Game* game = malloc(sizeof(Game));
    if(game)
    {
        game->max_cell = 2048;
        game->max_random = 4;
        game->random_seed = 0;
        game->random_state = 0;
        game->score = 0;
        game->status = GAME_STATUS_NEW;
        game->board = CreateBoard(4, 4);
        if(game->board == NULL)
        {
            free(game);
            game = NULL;
        }
    }
    return game;
}

void DestroyGame(Game* game)
{
    if(game == NULL) return;
    DestroyBoard(game->board);
    free(game);
}

void StartGame(Game* game)
{
    if(game == NULL) return;
    game->random_state = game->random_seed;
    if(game->status != GAME_STATUS_NEW)
    {
        game->status = GAME_STATUS_NEW;
        game->score = 0;
        ResetBoard(game->board);
    }
}

void NewGame(Game* game)
{
    if(game == NULL) return;
    game->random_seed = rand();
    StartGame(game);
}

void NextTurn(Game* game)
{
    if(game == NULL) return;
    uint32_t cell_count = game->board->rows * game->board->cols;
    uint32_t* is = malloc(sizeof(uint32_t) * cell_count);
    if(is == NULL) return;
    uint32_t n = 0, i;
    for(i = 0; i < cell_count; i++)
    {
        if(game->board->cells[i] == 0)
        {
            is[n] = i;
            n++;
        }
    }
    if(n > 0)
    {
        if(game->status = GAME_STATUS_NEW) game->status = GAME_STATUS_PLAYING;
        game->random_state = Random(game->random_state);
        uint32_t celli = game->random_state % n;
        game->random_state = Random(game->random_state);
        uint32_t cell = game->random_state % game->max_random;
        game->board->cells[is[celli]] = cell - (cell % 2);
    }
    else if(game->status != GAME_STATUS_WON)
    {
        game->status = GAME_STATUS_LOST;
    }
    free(is);
}
