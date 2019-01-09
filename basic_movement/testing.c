#include "movement.h"

// I dunno if there's a nicer way to do this
int mv_actor(int mv, struct actor *actor)
{
    int s = 0;
    // backup
    int b_row = actor->row;
    int b_col = actor->col;

    switch(mv)  {
        case KEY_MV_NE:
            --(actor->row);
            ++(actor->col);
            break;
        case KEY_MV_SE:
            ++(actor->row);
            ++(actor->col);
            break;
        case KEY_MV_NW:
            --(actor->row);
            --(actor->col);
            break;
        case KEY_MV_SW:
            ++(actor->row);
            --(actor->col);
            break;
        case KEY_MV_N:
            --(actor->row);
            break;
        case KEY_MV_S:
            ++(actor->row);
            break;
        case KEY_MV_E:
            ++(actor->col);
            break;
        case KEY_MV_W:
            --(actor->col);
            break;
    }
    if(!valid_pos(actor)) {
        s = 1;
        actor->row = b_row;
        actor->col = b_col;
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

int valid_pos(struct actor *actor)
{
    return
        actor->row != 0
        && actor->col != 0
        && actor->row != 24
        && actor->col != 80
        ;
}

int main()
{
    int s;
    int ch = 'e';
    struct actor jef;
    jef.row = 4;
    jef.col = 4;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    printw("peepee moments");
    mvaddch(jef.row, jef.col, '@');

    while(ch != ' ') {
        ch = getch();
        if(dkey(ch)) {
            mvaddch(jef.row, jef.col, ' ');
            s = mv_actor(dkey(ch), &jef);
        }
        mvaddch(jef.row, jef.col, '@');
        if(s) printw("you died");
        refresh();
    }

    endwin();
    return 0;
}
