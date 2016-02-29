#ifndef _2048_GAME_LOGIC_H
#define _2048_GAME_LOGIC_H

#include "board.h"
#include <stdint.h>

typedef enum
{   GAME_STATUS_NEW,
    GAME_STATUS_PLAYING,
    GAME_STATUS_WON,
    GAME_STATUS_LOST,
    GAME_STATUS_ERROR
} GameStatus;

typedef struct
{   int win_level;
    int random_max_level;
    uint32_t random_seed;
    uint32_t random;
    int score;
    GameStatus status;
    Board* board;
} Game;

/*
 * Creates a game with an empty board.
 * Returns a nullpointer if there was an error.
 * Errors:
 * See possible errors for CreateBoard
 * Failure to allocate memory
 */
Game* CreateGame(int rows, int cols);

/*
 * Frees the memory occupied by a game.
 */
void DestroyGame(Game* game);

/*
 * Starts the game based on the random seed (will restart an already started game).
 */
void StartGame(Game* game);

/*
 * Starts a new game (different random seed)
 */
void NewGame(Game* game);

/*
 * Initializes next turn.
 * Adds a random new cell in an empty slot on the game board.
 * If there is no room for new cells, status is set to GAME_STATUS_LOST,
 * unless the game status is already GAME_STATUS_WON.
 * If the game status is GAME_STATUS_NEW, it is set to GAME_STATUS_PLAYING.
 * If there was an error (while allocating memory) the game status is set to GAME_STATUS_ERROR.
 */
void NextTurn(Game* game);

/*
 * Pushes all cells to the left.
 * Cells that have the same values that have no other cells in between
 * are joined together to a single tile.
 * Game status is set to GAME_STATUS_WON if a cell reaches the maximum cell value.
 * Returns 1 if tiles have moved, 0 if not.
 */
int PushLeft(Game* game);

/*
 * Pushes all cells to the right.
 * Cells that have the same values that have no other cells in between
 * are joined together to a single tile.
 * Game status is set to GAME_STATUS_WON if a cell reaches the maximum cell value.
 * Returns 1 if tiles have moved, 0 if not.
 */
int PushRight(Game* game);

/*
 * Pushes all cells up.
 * Cells that have the same values that have no other cells in between
 * are joined together to a single tile.
 * Game status is set to GAME_STATUS_WON if a cell reaches the maximum cell value.
 * Returns 1 if tiles have moved, 0 if not.
 */
int PushUp(Game* game);

/*
 * Pushes all cells down.
 * Cells that have the same values that have no other cells in between
 * are joined together to a single tile.
 * Game status is set to GAME_STATUS_WON if a cell reaches the maximum cell value.
 * Returns 1 if tiles have moved, 0 if not.
 */
int PushDown(Game* game);

#endif
