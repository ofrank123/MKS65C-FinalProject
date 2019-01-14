#include "movement.h"

#define STATUS_SIZE 2

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

int input_handler(struct actor * jef, WINDOW * main_w, WINDOW * statusline) {
  int ch = getch();
  int s;
  if(dkey(ch)) {
    mvwaddch(main_w, jef->row, jef->col, ' '); //clear old @
    s = mv_actor(dkey(ch), jef);

    if(s) mvwprintw(statusline, 1, 1, "you died");
    else  wclear(statusline);
  }
  else if(ch == 27) {
    return 0;
  }
  return 1;
}

void draw(struct actor * jef, WINDOW * main_w, WINDOW * statusline) {
  mvwaddch(main_w, jef->row, jef->col, '@');
  box(main_w, 0, 0);
  box(statusline, 0, 0);

  wrefresh(main_w);
  wrefresh(statusline);
}

int main()
{
    int term_y, term_x, new_y, new_x;
    int s;
    int ch = '\0';
    struct actor jef;
    jef.row = 4;
    jef.col = 4;

    // curses initialization
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    // initialize windows
    getmaxyx(stdscr, term_y, term_x);
    WINDOW *field = newwin((term_y - 3), term_x, 0, 0);
    WINDOW *statusline = newwin(3, term_x, (term_y - 3), 0);
    refresh(); // let's not make that mistake again

    // some initial output
    mvaddch(jef.row, jef.col, '@');
    mvwprintw(statusline, 0, 0, "initialized");
    box(field, 0, 0);
    box(statusline, 0, 0);
    wrefresh(field);
    wrefresh(statusline);

    char running = 1;

    // main movement loop
    while(running) {
        // resize windows (this needs to be compartmentalized)
        getmaxyx(stdscr, new_y, new_x);
        if(new_y != term_y || new_x != term_x) {
            term_y = new_y;
            term_x = new_x;
            if(term_y >= 24 && term_x >= 80) {
                wresize(field, (term_y - 3), term_x);
                wresize(statusline, 3, term_x);
                mvwin(statusline, (term_y - 3), 0);
                wclear(stdscr);
                wclear(field);
                wclear(statusline);
                box(field, 0, 0);
                box(statusline, 0, 0);
            }
            else {
                wclear(field);
                mvwprintw(field, 1, 1, "Your window is too small!");
                mvwprintw(field, 3, 1, "Use 80x24 pls. (You have %ix%i.)",
                                term_x, term_y);
                wrefresh(field);
            }
        }

        running = input_handler(&jef, field, statusline);
        draw(&jef, field, statusline);
    }

    // curses is done
    endwin();
    return 0;
}
