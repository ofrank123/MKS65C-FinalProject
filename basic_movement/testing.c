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

    if(s) mvwprintw(statusline, 0, 0, "you died");
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

  wrefresh(main_w);
  wrefresh(statusline);
}

int main()
{
    int s;
    int ch = 'e';
    struct actor jef;
    jef.row = 4;
    jef.col = 4;

    // curses initialization
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    // initialize windows
    WINDOW *main_w = newwin((LINES - STATUS_SIZE), COLS, 0, 0);
    box(main_w, 0, 0);
    WINDOW *statusline = newwin(STATUS_SIZE, COLS, (LINES - STATUS_SIZE), 0);

    // some initial output
    wprintw(main_w, "LINES: %d COLS: %d", LINES, COLS);
    mvaddch(jef.row, jef.col, '@');
    mvwprintw(statusline, 0, 0, "initialized");
    wrefresh(main_w);
    wrefresh(statusline);
        // why doesn't it display if I uncomment these

    char running = 1;
    // main movement loop
    while(running) {
      running = input_handler(&jef, main_w, statusline);
      draw(&jef, main_w, statusline);
   }

    // curses is done
    endwin();
    return 0;
}
