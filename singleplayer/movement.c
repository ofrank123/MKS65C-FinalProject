#include "movement.h"

#ifndef MAX
    #define MAX(a,b) ((a) > (b) ? a : b)
#endif

#define BUILD_RADIUS 15

// I dunno if there's a nicer way to do this
int mv_actor(int mv, struct actor *actor, struct map *m)
{
    int s = 0;
    // backup
    int b_z = actor->z;
    int b_x = actor->x;
    int b_y = actor->y;
    int b_vy = actor->view_y;

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
    
    if(m->arr[actor->y][actor->z][actor->x] == '1') {
      // step up
      if(m->arr[(actor->y) - 1][actor->z][actor->x] == '0') {
        if(actor->view_y == actor->y)
            (actor->view_y)--;
        (actor->y)--;
      }
      else {
        s=1;
        actor->z = b_z;
        actor->x = b_x;
        actor->y = b_y;
        actor->view_y = b_vy;
      }
    }
    if(m->arr[actor->y][actor->z][actor->x] == '0') {
      // step down
      while(m->arr[(actor->y) + 1][actor->z][actor->x] == '0') {
        if(actor->view_y == actor->y)
            (actor->view_y)++;
        (actor->y)++;
   
   }
    }
    if(!within_limits(actor, m)) {
      s = 1;
      actor->z = b_z;
      actor->x = b_x;
      actor->y = b_y;
      actor->view_y = b_vy;
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
    if(cursor_dist(actor) > BUILD_RADIUS) {
        s = 1;
        actor->cursor_z = b_z;
        actor->cursor_x = b_x;
    }
    if(!((actor->cursor_z + actor->z) > 0
         && (actor->cursor_x + actor->x) > 0
         && (actor->cursor_z + actor->z) < m->z_size - 1
         && (actor->cursor_x + actor->x) < m->x_size - 1)) {
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
    pl->cursor_x = 0;
    pl->cursor_z = 0;
}

void enter_move(struct actor *pl)
{
    pl->mode = MODE_MOVE;
    curs_set(0);
}

void build_tile(struct actor *pl, struct map *m)
{
    // "real" cursor coords
    int real_cz = pl->cursor_z + pl->z;
    int real_cx = pl->cursor_x + pl->x;

    char *target_block = m->arr[pl->view_y][real_cz] + real_cx;

    // break block
    if(*target_block == '1') {
        *target_block = '0';
        ++(pl->blocks);
        // (send diff)
        
    }
    // place block
    else if(*target_block == '0' && pl->blocks > 0) {
        *target_block = '1';
        --(pl->blocks);
        // (send diff)
    }
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
        case 'a': return KEY_VUP;
        case 'z': return KEY_VDOWN;
        default:  return 0;
    }
}
// anyway the idea is it'll be hopefully easy to add more keybinds and stuff

int within_limits(struct actor *actor, struct map *m)
{
  return
    actor->z > 0
    && actor->x >= 0
    && actor->y > 0
    && actor->z <= m->z_size - 2
    && actor->x <= m->x_size - 1
    && actor->y <= m->y_size - 1
    ;
        // I will ideologically defend this hanging semicolon --Yoshi
}

int cursor_dist(struct actor *pl)
{
    return MAX(abs(pl->cursor_x), abs(pl->cursor_z));
}

int input_handler(struct actor *jef, struct map *m,
                  WINDOW *main_w, WINDOW *statusline) {
    int ch = wgetch(main_w);
    int d, s;
    int real_cz, real_cx;
    if(d = dkey(ch)) {
        if(d >= KEY_MV_S && d <= KEY_MV_N) {
            if(jef->mode == MODE_MOVE) {
                s = mv_actor(dkey(ch), jef, m);
            }
            else if(jef->mode == MODE_BUILD) {
                s = mv_cursor(dkey(ch), jef, m);
            }
        }
        else if(d == KEY_BUILD)
            enter_build(jef);
        else if(d == KEY_MMOVE)
            enter_move(jef);
        else if(d == KEY_MEDIT && jef->mode == MODE_BUILD)
            build_tile(jef, m);
        else if(d == KEY_VUP) {
            if(jef->view_y > 0)
                --(jef->view_y);
        }
        else if(d == KEY_VDOWN) {
            if(jef->view_y < m->y_size - 2)
                ++(jef->view_y);
        }

        if(jef->mode == MODE_MOVE) {
            wclear(statusline);
            mvwprintw(statusline, 1, 1,
                    "[MOVE %+i]  Inv:%i X:%i Y:%i Z:%i MapVal:%i",
                    (jef->y - jef->view_y), // view level difference
                    jef->blocks,            // blocks ie inventory
                    jef->x, jef->y, jef->z, // player coords
                    m->arr[jef->y][jef->z][jef->x]); // value there

        } else if(jef->mode == MODE_BUILD) {
            real_cz = jef->cursor_z + jef->z; // real cursor z
            real_cx = jef->cursor_x + jef->x; // real cursor x
            wclear(statusline);
            mvwprintw(statusline, 1, 1,
                    "[BUILD %+i] Inv:%i X:%i Y:%i Z:%i MapVal:%i",
                    (jef->y - jef->view_y),   // view level difference
                    jef->blocks,              // blocks
                    real_cx, jef->view_y, real_cz,  // cursor x,y,z
                    m->arr[jef->view_y][real_cz][real_cx]);  //value there
        }
    }
    else if(ch == 27) {
        return 0;
    }
    return 1;
}
