#include "game_ui.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{   InitUI();
    srand(time(NULL));
    Game* game = CreateGame(4, 4);
    if(game == NULL) return 1;
    UI* ui = CreateUI(game);
    if(ui == NULL) return 2;
    Run(ui);
    DestroyUI(ui);
    DestroyGame(game);
    QuitUI();
    return 0;
}
