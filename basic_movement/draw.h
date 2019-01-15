#ifndef DRAW_H
#define DRAW_H
#include <ncurses.h>
#include "player.h"
#include "map.h"

// draw all windows
void draw(struct actor *player, struct map *m,
          WINDOW *main_w, WINDOW *statusline);

// draw map to main window
void draw_map(struct actor *player, struct map *m, WINDOW *field);
#endif
