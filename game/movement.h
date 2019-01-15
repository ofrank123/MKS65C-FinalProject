#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "player.h"
#include "map.h"

#define KEY_MV_N	9
#define KEY_MV_S	1
#define KEY_MV_E	2
#define KEY_MV_W	3
#define KEY_MV_NE	4
#define KEY_MV_SE	5
#define KEY_MV_NW	6
#define KEY_MV_SW	7
#define KEY_BUILD	10
#define KEY_MMOVE   11
#define KEY_MEDIT   12
#define KEY_VDOWN   13
#define KEY_VUP     14

int within_limits(struct actor *, struct map *);

int cursor_dist(struct actor *);

int mv_actor(int mv, struct actor *, struct map *);

int mv_cursor(int mv, struct actor *, struct map *);

void enter_build(struct actor *);

void enter_move (struct actor *);

int build_tile(struct actor *, struct map *);

int dkey(int ch);

int input_handler(struct actor *player, struct map *m,
                  WINDOW *field, WINDOW *statusline);

#endif
