#ifndef _2048_GAME_UI_H
#define _2048_GAME_UI_H

#include "game_logic.h"

/*
 * Initializes the user interface
 */
void InitUI();

/*
 * Shuts down the user interface
 */
void QuitUI();

typedef struct
{   Game* game;
    uint8_t cell_width;
} UI;

/*
 * Creates a user interface for a game.
 * Returns a nullpointer if there was an error.
 */
UI* CreateUI(Game* game);

/*
 * Frees the memory occupied by a user interface.
 */
void DestroyUI(UI* ui);

/*
 * Run the game.
 */
void Run(UI* ui);

#endif
