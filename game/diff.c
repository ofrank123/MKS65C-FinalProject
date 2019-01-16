#include "diff.h"

int process_diff(struct diff *d, struct map *m, struct otherplayer *opl)
{
    if(d->type == DIFF_BLOCKON)
        return blockon_diff(d, m);
    else if(d->type == DIFF_BLOCKOFF)
        return blockoff_diff(d, m);
    else if(d->type == DIFF_PLAYERMV)
        return playermv_diff(d, opl);
    else 
        return -1;
}

int blockon_diff(struct diff *d, struct map *m)
{
    m->arr[d->y][d->z][d->x] = '1';
    return 1;
}

int blockoff_diff(struct diff *d, struct map *m)
{
    m->arr[d->y][d->z][d->x] = '0';
    return 1;
}

int playermv_diff(struct diff *d, struct otherplayer *opl)
{
    opl->x = d->x;
    opl->y = d->y;
    opl->z = d->z;
    return 1;
}
