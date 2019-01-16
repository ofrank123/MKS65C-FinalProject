#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "movement.h"
#include "draw.h"
#include "diff.h"
#include "player.h"


#define _XOPEN_SOURCE_EXTENDED 1

int main()
{
    int term_y, term_x, new_y, new_x;
    int res;
    int s;
    int ch = '\0';
    struct actor jef;
    jef.z = 100;
    jef.x = 100;
    jef.y = 4;
    jef.blocks = 32;
    jef.view_y = 4;
    jef.mode = MODE_MOVE;

    struct otherplayer opl;

    struct diff movediff;
    movediff.type = DIFF_PLAYERMV;

    setlocale(LC_ALL, "");

    mkfifo("g2h", 0644);
    mkfifo("h2g", 0644);
    printf("waiting for guest....\n");
    int write_pipe = open("h2g", O_WRONLY);
    printf("host2guest pipe opened....\n");

    // import map
    struct map *main_map = read_map("map.map");

    printf("transmitting map....\n");
    // SEND MAP
    int y, z;
    for(y = 0; y < main_map->y_size; ++y) {
        for(z = 0; z < main_map->z_size; ++z) {
            res = write(write_pipe, main_map->arr[y][z],
                    sizeof(char) * (main_map->x_size));
            printf("transmitted row [%i][%i], %i bytes sent.\n", y, z, res);
        }
    }
    printf("map transmitted.\n");


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
    draw_map(&jef, main_map, &opl, field);
    wrefresh(field);
    wrefresh(statusline);

    char running = 1;
    // main movement loop
    while(running) {
        ch = wgetch(field);
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
        } // end resize windows

        // // send diffs here
        // movediff.x = jef.x;
        // movediff.y = jef.y;
        // movediff.z = jef.z;
        // write(write_pipe, &movediff, sizeof(struct diff));

        // receive diffs here, in theory

        if(ch != ERR)
            running = input_handler(&jef, main_map, field, statusline, ch,
                            write_pipe);
        draw(&jef, main_map, &opl, field, statusline);

        // // PIPE WRITING // // 
        // def_prog_mode();
        // endwin();

        // char buf[16];
        // strcpy(buf, "test\n");
        // write(write_pipe, (void *) buf, 6);

        // reset_prog_mode();
        // refresh();
    }

    // curses is done
    save_map(main_map, "map.map");
    free_map(main_map);
    endwin();
    return 0;
}
