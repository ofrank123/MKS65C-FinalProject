#include <locale.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "movement.h"
#include "draw.h"
#include "diff.h"
#include "player.h"

#define _XOPEN_SOURCE_EXTENDED 1

int main()
{
    int term_y, term_x, new_y, new_x;
    int s;
    int ch = '\0';
    struct actor jef;
    struct diff in_diff;
    jef.z = 100;
    jef.x = 100;
    jef.y = 4;
    jef.blocks = 32;
    jef.view_y = 4;
    jef.mode = MODE_MOVE;

    setlocale(LC_ALL, "");

    struct otherplayer opl;
    opl.z = 110;
    opl.x = 110;
    opl.y = 4;

    mkfifo("g2h", 0644);
    mkfifo("h2g", 0644);
    int read_pipe  = open("h2g", O_RDONLY);
    int write_pipe = open("g2h", O_WRONLY);

    struct map *m = malloc(sizeof(struct map));
    m->x_size = 256;
    m->y_size = 10;
    m->z_size = 256;
    m->arr = malloc(sizeof(char **) * m->y_size);

    // RECEIVE MAP
    int y, z, res;
    for(y = 0; y < m->y_size; ++y) {
        m->arr[y] = malloc(sizeof(char *) * m->z_size);
        for(z = 0; z < m->z_size; ++z) {
            m->arr[y][z] = malloc(sizeof(char) * m->x_size);
            res = read(read_pipe, m->arr[y][z], sizeof(char) * m->x_size);
            printf("received row [%i][%i], %i bytes, received.\n", y, z, res);
        }
    }
    close(read_pipe);
    read_pipe = open("h2g", O_RDONLY | O_NONBLOCK);

    sleep(1);

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
    box(field, 0, 0);
    box(statusline, 0, 0);
    draw_map(&jef, m, &opl, field);
    wrefresh(field);
    wrefresh(statusline);

    char running = 1;
    // main movement loop
    while(running) {
        ch = wgetch(field);
        // resize windows (this needs to be compartmentalized)
        getmaxyx(stdscr, new_y, new_x);
        if(new_y != term_y || new_x != term_x) {
            ch = wgetch(field);
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
        } // end resize windows

        // receive diffs here, in theory
        while((res = read(read_pipe, &in_diff, sizeof(struct diff))) != -1)
            process_diff(&in_diff, m, &opl);

        // main input/redraw loop
        if(ch != ERR)
            running = input_handler(&jef, m, field, statusline, ch, write_pipe);
        draw(&jef, m, &opl, field, statusline);
    }

    // curses is done
    free_map(m);
    endwin();

    return 0;
}
