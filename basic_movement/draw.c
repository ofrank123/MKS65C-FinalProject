#include "draw.h"

//Draw windows
void draw(struct actor * player, WINDOW * main_w, WINDOW * statusline) {
  mvwaddch(main_w, player->z, player->x, '@');
  box(main_w, 0, 0);
  box(statusline, 0, 0);

  wrefresh(main_w);
  wrefresh(statusline);
}
