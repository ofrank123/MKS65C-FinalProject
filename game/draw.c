#include "draw.h"

//Draw windows
void draw(struct actor *player, struct map *m, struct otherplayer *opl,
          WINDOW *main_w, WINDOW *statusline)
{
    int field_z, field_x;
    getmaxyx(main_w, field_z, field_x);

    box(main_w, 0, 0);
    box(statusline, 0, 0);
    draw_map(player, m, opl, main_w);

    wrefresh(statusline);
    wrefresh(main_w);
}

void draw_char(WINDOW * field, struct map * m, int dz, int dx, int x, int y, int z) {
  char c[8];
  if(m->arr[y][z][x] == '1') {
    if(m->arr[y-1][z][x] == '0') {
      if(air_neighbors(m, x, y, z)) {
        strcpy(c, BLOCK_UP);
      } else {
        strcpy(c, BLOCK_ONEABOVE);
      }
    } else {
      strcpy(c, BLOCK_SOLID);
    }
  }
  else if(m->arr[y][z][x] == '0') {
    if(m->arr[y+1][z][x] == '1') {
      if(air_neighbors(m, x, y + 1, z)) {
        strcpy(c, BLOCK_DOWN);
      } else {
        strcpy(c, BLOCK_FOOT);
      }
    } else if (m->arr[y+2][z][x] == '1') {
      strcpy(c, BLOCK_UNDER);
    } else {
      strcpy(c, BLOCK_AIR);
    }
  }
  mvwaddstr(field, dz, dx, c);
}

char neighbor_limits(struct map *m, int x, int z) {
  return
    z >= 1
    && x >= 1
    && z < m->z_size - 1
    && z < m->x_size - 1
    ;
}

int air_neighbors(struct map * m, int x, int y, int z) {
  for(int cx = -1; cx <= 1; cx++) {
    for(int cz = -1; cz <= 1; cz++) {
      if(neighbor_limits(m, x + cx, z + cz)){
        if(m->arr[y][z + cz][x + cx] == '0') {
          return 1;
        }
      }
    }
  }
  return 0;
}

// draw map to main window, centered around player (doesn't refresh)
void draw_map(struct actor *pl, struct map *m, struct otherplayer *opl,
        WINDOW *field)
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
          //mvwaddch(field, 1+z, 1+x, m->arr[pl->view_y][view_z + z][view_x + x]);
            draw_char(field, m, 1+z, 1+x, view_x + x, pl->view_y, view_z + z);
                        /* THIS USES pl->view_y */
        }
    }

    // draw the otherplayer if they're on screen
    if(opl->y == pl->view_y
        && opl->x >= view_x && opl->x < view_x + field_x
        && opl->z >= view_z && opl->z < view_z + field_z
        ) {         // cursed trailing delimiter returns
        mvaddch((opl->z - view_z), (opl->x - view_x), '@');

    }

    // draw the player, if we're not on a different layer
    if(pl->view_y == pl->y)
        mvwaddstr(field, pl_z, pl_x, "\xe2\x98\xba");

    // move the cursor if we're in build mode
    if(pl->mode == MODE_BUILD)
        wmove(field, (pl_z + pl->cursor_z), (pl_x + pl->cursor_x));

}
