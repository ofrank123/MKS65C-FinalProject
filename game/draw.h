#ifndef DRAW_H
#define DRAW_H
#include <ncurses.h>
#include "player.h"
#include "map.h"
#include <wchar.h>
#include "blocks.h"

// draw all windows
void draw(struct actor *player, struct map *m, struct otherplayer *opl,
          WINDOW *main_w, WINDOW *statusline);

// draw map to main window
void draw_map(struct actor *player, struct map *m, struct otherplayer *opl,
        WINDOW *field);

int air_neighbors(struct map * m, int x, int y, int z);

#endif
