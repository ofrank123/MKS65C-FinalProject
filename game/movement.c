#include "movement.h"

// I dunno if there's a nicer way to do this
int mv_actor(int mv, struct actor *actor, struct map *m)
{
    int s = 0;
    // backup
    int b_z = actor->z;
    int b_x = actor->x;

    switch(mv)  {
        case KEY_MV_NE:
            --(actor->z);
            ++(actor->x);
            break;
        case KEY_MV_SE:
            ++(actor->z);
            ++(actor->x);
            break;
        case KEY_MV_NW:
            --(actor->z);
            --(actor->x);
            break;
        case KEY_MV_SW:
            ++(actor->z);
            --(actor->x);
            break;
        case KEY_MV_N:
            --(actor->z);
            break;
        case KEY_MV_S:
            ++(actor->z);
            break;
        case KEY_MV_E:
            ++(actor->x);
            break;
        case KEY_MV_W:
            --(actor->x);
            break;
    }
    if(!valid_pos(actor, m)) {
        s = 1;
        actor->z = b_z;
        actor->x = b_x;
    }
    if(m->arr[actor->y][actor->z][actor->x] == '1') {
      if(m->arr[(actor->y) - 1][actor->z][actor->x] == '0') {
        (actor->y)--;
      }
      else {
        s=1;
        actor->z = b_z;
        actor->x = b_x;
      }
    }
    if(m->arr[actor->y][actor->z][actor->x] == '0') {
      if(m->arr[(actor->y) + 1][actor->z][actor->x] == '0') {
        (actor->y)++;
      }
    }
    return s;
}

// I guess this just compiles into a lookup table so it's pretty alright
int dkey(int ch)
{
    switch(ch) {
        case 'h': return KEY_MV_W;
        case 'j': return KEY_MV_S;
        case 'k': return KEY_MV_N;
        case 'l': return KEY_MV_E;
        case 'y': return KEY_MV_NW;
        case 'u': return KEY_MV_NE;
        case 'b': return KEY_MV_SW;
        case 'n': return KEY_MV_SE;
        default:  return 0;
    }
}
// anyway the idea is it'll be hopefully easy to add more keybinds and stuff

int valid_pos(struct actor *actor, struct map *m)
{
    return
        actor->z >= 0
        && actor->x >= 0
        && actor->z <= m->z_size - 1
        && actor->x <= m->x_size - 1
        ;
        // I will ideologically defend this hanging semicolon --Yoshi
}

int input_handler(struct actor *jef, struct map *m,
                  WINDOW *main_w, WINDOW *statusline) {
  int ch = getch();
  int s;
  if(dkey(ch)) {
    mvwaddch(main_w, jef->z, jef->x, ' '); //clear old @
    s = mv_actor(dkey(ch), jef, m);

    if(s) mvwprintw(statusline, 1, 1, "edge of the world");
    else  wclear(statusline);
  }
  else if(ch == 27) {
    return 0;
  }
  return 1;
}
