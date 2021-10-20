#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <curses.h>
#include <panel.h>
#include <form.h>
#include <string.h>

#include "todo.h"

int main(int argc, char *argv[])
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    ENTRY *entries;
    MY_WINDOW *todo;
    MY_WINDOW *add_form_window; // The window that stores the add form
    char *title = "TODO List Manager v0.1";

    // Initializing the windows
    add_form_window = init_addFormWindow();
    todo = init_todoWindow();
    update_panels();

    // Fabricating todo list entries
    entries = entry_fromFile("entries.txt");

    // Printing the title
    mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);

    // Drawing the todo window
    draw_addForm(add_form_window);
    draw_window(todo);
    // Drawing the list of entries
    draw_entries(todo, entries);
    fprintf(stderr, "1\n");
    // Refresh virtual and physical windows
    update_panels();
    doupdate();

    char c;
    while ((c = getch()) != 'q')
    {
        switch (c)
        {
            case 'a': // Add an item
                form_handle(add_form_window, &entries, ENTRY_INSERT);
                top_panel(todo->panel);
                break;
            case 'd': // Delete an item
                form_handle(add_form_window, &entries, ENTRY_DELETE);
                top_panel(todo->panel);
                break;
            default:
                break;
        }

        // Drawing the todo window
        draw_addForm(add_form_window);
        draw_window(todo);

        // Drawing the list of entries
        draw_entries(todo, entries);
        fprintf(stderr, "2\n");

        // Refresh virtual and physical windows
        update_panels();
        doupdate();
    }

    // After user is done modifying entries, save the state of the entries
    // to a file
    entry_toFile(entries, "entries.txt");

    // Free things for the form window
    unpost_form(add_form_window->form);
    free_form(add_form_window->form);
    free_field(add_form_window->fields[0]);
    free(add_form_window->fields);
    delwin(add_form_window->win);
    del_panel(add_form_window->panel);
    free(add_form_window);

    del_panel(todo->panel);
    delwin(todo->win);
    free(todo);
    entry_free(entries);
    endwin();
	return EXIT_SUCCESS;	
}

