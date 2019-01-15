#include <locale.h>
#include "movement.h"
#include "draw.h"
#include "player.h"

#define _XOPEN_SOURCE_EXTENDED 1

int main()
{
    int term_y, term_x, new_y, new_x;
    int s;
    int ch = '\0';
    struct actor jef;
    jef.z = 100;
    jef.x = 100;
    jef.y = 4;
    jef.view_y = 4;
    jef.mode = MODE_MOVE;

    setlocale(LC_ALL, "");

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

    // import map
    struct map *main_map = read_map("map.m");
    //struct map *main_map = NULL;

    // some initial output
    wprintw(field, "LINES: %d XS: %d", term_x, term_y);
    mvwprintw(statusline, 0, 0, "initialized");
    box(field, 0, 0);
    box(statusline, 0, 0);
    draw_map(&jef, main_map, field);
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
            if(term_y >= 15 && term_x >= 60) {
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
                mvwprintw(field, 3, 1, "Use 60x15 pls. (You have %ix%i.)",
                                term_x, term_y);
                wrefresh(field);
            }
        }
        running = input_handler(&jef, main_map, field, statusline);
        draw(&jef, main_map, field, statusline);
    }

    // curses is done
    free_map(main_map);
    endwin();
    return 0;
}
