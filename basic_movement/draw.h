#ifndef DRAW_H
#define DRAW_H
#include <ncurses.h>
#include "player.h"

void draw(struct actor * player, WINDOW * main_w, WINDOW * statusline);
#endif
