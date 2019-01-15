#include "movement.h"
#include "draw.h"
#include "player.h"

#define STATUS_SIZE 2

int main()
{
    int term_y, term_x, new_y, new_x;
    int s;
    int ch = '\0';
    struct actor jef;
    jef.z = 4;
    jef.x = 4;

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
    wprintw(field, "LINES: %d XS: %d", term_x, term_y);
    mvaddch(jef.z, jef.x, '@');
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
