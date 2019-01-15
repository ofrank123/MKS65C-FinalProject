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
    if(!within_limits(actor, m)) {
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

// really terrible coding right here
int mv_cursor(int mv, struct actor *actor, struct map *m)
{
    int s = 0;
    // backup
    int b_z = actor->cursor_z;
    int b_x = actor->cursor_x;

    switch(mv)  {
        case KEY_MV_NE:
            --(actor->cursor_z);
            ++(actor->cursor_x);
            break;
        case KEY_MV_SE:
            ++(actor->cursor_z);
            ++(actor->cursor_x);
            break;
        case KEY_MV_NW:
            --(actor->cursor_z);
            --(actor->cursor_x);
            break;
        case KEY_MV_SW:
            ++(actor->cursor_z);
            --(actor->cursor_x);
            break;
        case KEY_MV_N:
            --(actor->cursor_z);
            break;
        case KEY_MV_S:
            ++(actor->cursor_z);
            break;
        case KEY_MV_E:
            ++(actor->cursor_x);
            break;
        case KEY_MV_W:
            --(actor->cursor_x);
            break;
    }
    if(!cursor_limits(actor, m)) {
        s = 1;
        actor->cursor_z = b_z;
        actor->cursor_x = b_x;
    }
    return s;
}

void enter_build(struct actor *pl)
{
    pl->mode = MODE_BUILD;
    curs_set(1);
    pl->cursor_x = pl->x;
    pl->cursor_z = pl->z;
}

void enter_move(struct actor *pl)
{
    pl->mode = MODE_MOVE;
    curs_set(0);
}

int build_tile(struct actor *pl, struct map *m)
{
    return 0;
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
        case ' ': return KEY_MEDIT;
        case 'i': return KEY_BUILD;
        case 'o': return KEY_MMOVE;
        default:  return 0;
    }
}
// anyway the idea is it'll be hopefully easy to add more keybinds and stuff

int within_limits(struct actor *actor, struct map *m)
{
    return
        actor->z >= 0
        && actor->x >= 0
        && actor->z <= m->z_size - 1
        && actor->x <= m->x_size - 1
        ;
        // I will ideologically defend this hanging semicolon --Yoshi
}

// aaaaaaaaaaaaaaa
int cursor_limits(struct actor *actor, struct map *m)
{
    return
        actor->cursor_z >= 0
        && actor->cursor_x >= 0
        && actor->cursor_z <= m->z_size - 1
        && actor->cursor_x <= m->x_size - 1
        ;
        // I will ideologically defend this hanging semicolon --Yoshi
}

int input_handler(struct actor *jef, struct map *m,
                  WINDOW *main_w, WINDOW *statusline) {
    int ch = getch();
    int d;
    int s;
    if(d = dkey(ch)) {
        if(d >= KEY_MV_S && d <= KEY_MV_N) {
            if(jef->mode == MODE_MOVE)
                s = mv_actor(dkey(ch), jef, m);
            else if(jef->mode == MODE_BUILD)
                s = mv_cursor(dkey(ch), jef, m);
        }
        else if(d == KEY_BUILD)
            enter_build(jef);
        else if(d == KEY_MMOVE)
            enter_move(jef);
        else if(d == KEY_MEDIT && jef->mode == MODE_BUILD)
            build_tile(jef, m);
    
        if(s) mvwprintw(statusline, 1, 1, "edge of the world");
        else  wclear(statusline);
    }
    else if(ch == 27) {
        return 0;
    }
    return 1;
}
