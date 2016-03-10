#include "game_ui.h"
#include <curses.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

void InitUI()
{   initscr();
    if(has_colors())
    {   start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_BLACK, COLOR_CYAN);
        init_pair(3, COLOR_BLACK, COLOR_GREEN);
        init_pair(4, COLOR_BLACK, COLOR_YELLOW);
        init_pair(5, COLOR_BLACK, COLOR_RED);
        init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(7, COLOR_BLACK, COLOR_BLUE);
        init_pair(8, COLOR_WHITE, COLOR_BLACK);
    }
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void QuitUI()
{   endwin();
}

UI* CreateUI(Game* game)
{   if(game == NULL) return NULL;
    UI* ui = malloc(sizeof(UI));
    if(ui == NULL) return NULL;
    ui->game = game;
    ui->cell_width = 4;
    return ui;
}

void DestroyUI(UI* ui)
{   if(ui == NULL) return;
    free(ui);
}

static void DisplayCell(int row, int col, uint8_t width,
                          short color, int val)
{   char buf[16];
    if(val > 0)
    {   sprintf(buf, "%d", val);
        int len = strlen(buf);
        if(len > width)
        {   buf[width] = '\0';
            buf[width - 1] = '$';
        } else if(len < width)
        {   int i;
            for(i = 1; i <= len; i++) buf[width - i] = buf[len - i];
            for(i = 0; i < width - len; i++) buf[i] = ' ';
            buf[width] = '\0';
        }
    }
    if(has_colors()) attron(COLOR_PAIR(color));
    row *= 3;
    width += 2;
    col *= width;
    int i;
    for(i = 0; i < width; i++) mvaddch(row, col + i, ' ');
    row++;
    if(val > 0)
    {   mvaddch(row, col, ' ');
        mvaddstr(row, col + 1, buf);
        mvaddch(row, col + width - 1, ' ');
    } else
        for(i = 0; i < width; i++) mvaddch(row, col + i, ' ');
    row++;
    for(i = 0; i < width; i++) mvaddch(row, col + i, ' ');
	row++;
	move(row, 0);
}

static void DisplayGame(UI* ui)
{   erase();
	int* cells = ui->game->board->cells;
    int rows = ui->game->board->rows, cols = ui->game->board->cols;
    int row, col;
    for(row = 0; row < rows; row++)
    {   for(col = 0; col < cols; col++)
        {   int i = row * cols + col;
            if(cells[i] == 0) DisplayCell(row, col, ui->cell_width, 8, 0);
            else
            {   short color;
                switch(cells[i])
                {
                case 2:
                case 4:
                    color = 1;
                    break;
                case 8:
                case 16:
                    color = 2;
                    break;
                case 32:
                case 64:
                    color = 3;
                    break;
                case 128:
                case 256:
                    color = 4;
                    break;
                case 512:
                case 1024:
                    color = 5;
                    break;
                case 2048:
                    color = 6;
                    break;
                default:
                    color = 7;
                }
                DisplayCell(row, col, ui->cell_width, color, cells[i]);
            }
        }
    }
	if(has_colors()) attron(COLOR_PAIR(8));
	printw("Score: %d", ui->game->score);
    refresh();
}

void Run(UI* ui)
{   if(ui == NULL) return;
    NewGame(ui->game);
    NextTurn(ui->game);
    int c, running = 1;
    while(running && ui->game->status != GAME_STATUS_ERROR)
    {   DisplayGame(ui);
        c = getch();
        switch(c)
        {
        case KEY_LEFT:
			if(ui->game->status != GAME_STATUS_LOST)
			{	if(PushLeft(ui->game)) NextTurn(ui->game);
				else CheckTurn(ui->game);
			}
            break;
        case KEY_RIGHT:
			if(ui->game->status != GAME_STATUS_LOST)
            {	if(PushRight(ui->game)) NextTurn(ui->game);
				else CheckTurn(ui->game);
			}
			break;
        case KEY_UP:
			if(ui->game->status != GAME_STATUS_LOST)
			{	if(PushUp(ui->game)) NextTurn(ui->game);
				else CheckTurn(ui->game);
			}
			break;
        case KEY_DOWN:
			if(ui->game->status != GAME_STATUS_LOST)
			{	if(PushDown(ui->game)) NextTurn(ui->game);
				else CheckTurn(ui->game);
			}
			break;
        case 'q':
            running = 0;
            break;
		case 'n':
			NewGame(ui->game);
			NextTurn(ui->game);
			break;
		case 'r':
			StartGame(ui->game);
			NextTurn(ui->game);
			break;
        default:
            break;
        }
    }
}
