#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "player.h"

#define KEY_MV_N	9
#define KEY_MV_S	1
#define KEY_MV_E	2
#define KEY_MV_W	3
#define KEY_MV_NE	4
#define KEY_MV_SE	5
#define KEY_MV_NW	6
#define KEY_MV_SW	7

int valid_pos(struct actor *);

int mv_actor(int mv, struct actor *);

int dkey(int ch);

int input_handler(struct actor * player, WINDOW * field, WINDOW * statusline);

#endif
