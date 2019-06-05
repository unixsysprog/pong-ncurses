#include <stdlib.h>
#include <stdbool.h>

#include <curses.h>
#include <locale.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include "objects.h"
#include "render.h"
#include "update.h"

#include "config.h"
#include "util.h"

static struct board* main_board = NULL;

void sanity_check(void) {
    /*
     * make sure everything is ok before starting main program.
     * mainly for ensuring window isn't too small for game.
     */

    int x;
    int y;
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        // if ioctl fails, something's seriously wrong, so don't continue
        perror("sanity_check");
        exit(EXIT_FAILURE);
    }

    x = ws.ws_col;
    y = ws.ws_row;

    TRACE("Found window dimensions: %dx%d\n", x, y);

    if (x < PONG_REQUIRED_X || y < PONG_REQUIRED_Y) {
        ERROR("sanity_check: Window size is too small "
              "(minimum required dimensions: %dx%d).\n",
              PONG_REQUIRED_X,
              PONG_REQUIRED_Y);
        exit(EXIT_FAILURE);
    }
}

void setup_curses(void) {
    /*
     * initializes and configures curses window.
     */

    setlocale(LC_ALL, "en_US.utf-8");
    initscr();

    if (has_colors()) {
        start_color();
        init_pair(PONG_MAIN_COLOR, PONG_MAIN_FG, PONG_MAIN_BG);
        bkgd(COLOR_PAIR(PONG_MAIN_COLOR));
    }

    cbreak();       // game must have inputs immediately available
    noecho();       // games shouldn't display inputs (in most cases)
    curs_set(0);    // make cursor invisible when moving it around screen

    scrollok(stdscr, FALSE);
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
}

void cleanup(void) {
    /*
     * tidying function invoked via atexit()
     */

    endwin();
    board_destroy(main_board);
}

int main(void) {
    sanity_check();
    setup_curses();
    atexit(cleanup);

    main_board = board_init(true);

    while (true) {
        update_board(main_board);
        render_board(main_board);
        refresh();
        getch();
    }

    return 0;
}
