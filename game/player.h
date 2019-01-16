#ifndef PLAYER_H
#define PLAYER_H

#define MODE_BUILD  2
#define MODE_MOVE   4

struct actor
{
    int  x, y, z;
    int view_y;
    char mode;
    int blocks;
    int cursor_x, cursor_z;
};

struct otherplayer
{
    int x, y, z;
};

#endif 
