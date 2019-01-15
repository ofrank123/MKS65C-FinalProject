#include "draw.h"
#include <wchar.h>

//Draw windows
void draw(struct actor *player, struct map *m,
          WINDOW *main_w, WINDOW *statusline)
{
    int field_z, field_x;
    getmaxyx(main_w, field_z, field_x);

    box(main_w, 0, 0);
    box(statusline, 0, 0);
    draw_map(player, m, main_w);


    //mvwaddch(main_w, player->z, player->x, '@');
    //mvwaddch(main_w, (field_z / 2), (field_x / 2), '@');

    wrefresh(statusline);
    wrefresh(main_w);
}

// draw map to main window, centered around player (doesn't refresh)
void draw_map(struct actor *pl, struct map *m, WINDOW *field)
{

    int field_z, field_x;
    getmaxyx(field, field_z, field_x);
    // to adjust for borders
    field_z -= 2;
    field_x -= 2;

    int pl_z = field_z / 2;
    int pl_x = field_x / 2;
    // now that's what I call elegant coding
    
    // find the viewport's real coords (on map)
    int view_z = pl->z - (field_z) / 2 + 1;
    int view_x = pl->x - (field_x) / 2 + 1;

    // check for edge cases (haha pun)
    // z edges
    if(view_z < 0) {
        // limit the viewport to the edge and push the player display instead
        pl_z += view_z;
        view_z = 0;
    }
    else if(view_z + field_z >= m->z_size) {
        pl_z += (view_z + field_z) - m->z_size + 1;
        view_z = m->z_size - field_z - 1;
    }
    // x edges
    if(view_x < 0) {
        pl_x += view_x;
        view_x = 0;
    }
    else if(view_x + field_x >= m->x_size) {
        pl_x += (view_x + field_x) - m->x_size + 1;
        view_x = m->x_size - field_x - 1;
    }

    // now draw the viewport
    int x, z;
    for(z = 0; z < field_z; ++z) {
        for(x = 0; x < field_x; ++x) {
            mvwaddch(field, 1+z, 1+x,
                    m->arr[pl->view_y][view_z + z][view_x + x]);
                        /* THIS USES pl->view_y */
        }
    }

    // draw the player, if we're not on a different layer
    if(pl->view_y == pl->y)
        mvwaddstr(field, pl_z, pl_x, "\xe2\x98\xba");

    // draw the cursor if we're in build mode
    if(pl->mode == MODE_BUILD)
        wmove(field, (pl_z + pl->cursor_z), (pl_x + pl->cursor_x));

}
