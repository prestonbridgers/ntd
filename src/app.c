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

void draw_entries(MY_WINDOW *arg, ENTRY *head)
{
    int xpos = 1;
    int ypos = 1;
    char entry_str[MAX_ENTRY_NAME_SIZE];
    ENTRY *cur;

    werase(arg->win);
    for (cur = head; cur != NULL; cur = cur->next)
    {
        if (cur->isDone)
            strncpy(entry_str, "[x] ", MAX_ENTRY_NAME_SIZE);
        else
            strncpy(entry_str, "[ ] ", MAX_ENTRY_NAME_SIZE);

        strncat(entry_str, cur->name, MAX_ENTRY_NAME_SIZE - 5);
        mvwaddstr(arg->win, ypos, xpos, entry_str);
        ypos++;
    }

    // NOTE: May not need this
    box(arg->win, '|', '-');
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
    todo->ypos = (LINES / 2) - (todo->height / 2) + 1;
    todo->win = newwin(todo->height, todo->width,
                       todo->ypos, todo->xpos);
    todo->panel = new_panel(todo->win);

    // Fabricating todo list entries
    ENTRY *entries = entry_fromFile("entries.txt");

    // Printing the title
    char *title = "TODO List Manager v0.1";
    mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);

    // Drawing the todo window
    draw_window(todo);

    // Drawing the list of entries
    draw_entries(todo, entries);

    // Refresh virtual and physical windows
    wnoutrefresh(stdscr);
    wnoutrefresh(todo->win);
    doupdate();

    getch();

    del_panel(todo->panel);
    delwin(todo->win);
    free(todo);
    entry_free(entries);
    endwin();
	return EXIT_SUCCESS;	
}

