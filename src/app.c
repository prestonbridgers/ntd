#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <panel.h>
#include <string.h>

#include "todo.h"

typedef struct
{
    WINDOW *win;
    PANEL *panel;
    char *name;
    int width;
    int height;
    int xpos;
    int ypos;
} MY_WINDOW;

void draw_window(MY_WINDOW *arg)
{
    box(arg->win, '|', '-');
    mvwaddstr(arg->win, 0, (arg->width / 2) - (strlen(arg->name) / 2),
              arg->name);
    return;
}

int main(int argc, char *argv[])
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    fprintf(stderr, "COLS : %d\n", COLS);
    fprintf(stderr, "LINES: %d\n", LINES);

    MY_WINDOW *todo = (MY_WINDOW*) malloc(sizeof(*todo));
    todo->name = "TODO Items";
    todo->width       = COLS - 2;
    todo->height      = LINES - 3;
    todo->xpos = (COLS / 2) - (todo->width / 2);
    todo->ypos = (LINES / 2) - (todo->height / 2);
    todo->win = newwin(todo->height, todo->width,
                       todo->ypos, todo->xpos);

    // Printing the title
    char *title = "TODO List Manager v0.1";
    mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);

    // Drawing the todo window
    draw_window(todo);

    // Refresh virtual and physical windows
    wnoutrefresh(stdscr);
    wnoutrefresh(todo->win);
    doupdate();

    getch();
    endwin();
	return EXIT_SUCCESS;	
}

