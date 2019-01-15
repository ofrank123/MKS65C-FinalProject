#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define KEY_MV_N	9
#define KEY_MV_S	1
#define KEY_MV_E	2
#define KEY_MV_W	3
#define KEY_MV_NE	4
#define KEY_MV_SE	5
#define KEY_MV_NW	6
#define KEY_MV_SW	7

struct actor
{
  int x, y, z;
};

int valid_pos(struct actor *);

int mv_actor(int mv, struct actor *);

int dkey(int ch);

#endif
