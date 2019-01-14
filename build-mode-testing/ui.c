#include "ui.h"


int main()
{
    int term_y, term_x, new_y, new_x;
    int ch = '\0';

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

    // initial output I guess
    box(field, 0, 0);
    box(statusline, 0, 0);
    wrefresh(field);
    wrefresh(statusline);

    // something to deal with the map

    // main game loop
    while(ch != ' ') {
        // resize windows
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
                wprintw(field, "Your window is too small! Please use at least");
                wprintw(field, "an 80x24 terminal. (You have %ix%i.)",
                                term_x, term_y);
                wrefresh(field);
            }
        }

        // get input
        ch = getch();

        // convert to action
        // take action
        // [ send diffs ]
        // [ get server updates ] 
        // refill viewport
                
        // redraw
        wrefresh(field);
        wrefresh(statusline);
    }

    // quitting content
    endwin();
    return 0;
}
