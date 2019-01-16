#ifndef DIFF_H
#define DIFF_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "player.h"
#include "map.h"

#define DIFF_BLOCKON    1
#define DIFF_PLAYERMV   2
#define DIFF_BLOCKOFF   3

struct diff
{
    char type;
    int x, y, z;
};

int process_diff(struct diff *d, struct map *m, struct otherplayer *opl);

int blockon_diff(struct diff *d, struct map *m);

int blockoff_diff(struct diff *d, struct map *m);

int playermv_diff(struct diff *d, struct otherplayer *opl);

#endif
