#include <stdio.h>
#include <stdlib.h>
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

    // Test trimwhitespace
    char *meme = "     this is a whitespaced meme       ";
    char *after = trim_whitespaces(meme);
    fprintf(stderr, "testing whitespaces\n");
    fprintf(stderr, "before whitespace: %s\n", meme);
    fprintf(stderr, "after whitespace : %s\n", after);

    // Printing the title
    mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);

    // Drawing the todo window
    draw_addForm(add_form_window);
    draw_window(todo);
    // Drawing the list of entries
    draw_entries(todo, entries);
    // Refresh virtual and physical windows
    update_panels();
    doupdate();

    char c;
    while ((c = getch()) != 'q')
    {

        switch (c)
        {
            case 'a': // Add an item
                top_panel(add_form_window->panel);
                update_panels();
                doupdate();
                curs_set(1);
                // Enter form input loop
                int f;
                int done = 0;
                while (!done)
                {
                    f = wgetch(add_form_window->win);
                    switch (f)
                    {
                        case 10: // ENTER
                            fprintf(stderr, "User pressed ENTER in the form\n");
                            // ADD ENTRY TO THE LIST HERE
                            //FIELD **fields = form_fields(add_form_window->form);
                            char *field_buf = field_buffer(add_form_window->fields[0], 0);
                            char *field_buf_trimmed = trim_whitespaces(field_buf);

                            fprintf(stderr, "field_buf: %s\n", field_buf);
                            fprintf(stderr, "field_buf_trimmed: %s\n", field_buf_trimmed);

                            todo_insert(&entries, entry_create(field_buf_trimmed, 0));
                            top_panel(todo->panel);
                            form_driver(add_form_window->form, REQ_CLR_FIELD);
                            done = 1;
                            break;
                        case KEY_BACKSPACE: //BACKSPACE
                            fprintf(stderr, "User pressed BACKSPACE in the form\n");
                            form_driver(add_form_window->form, REQ_DEL_PREV);
                            break;
                        default:
                            form_driver(add_form_window->form, f);
                            break;
                    }
                    update_panels();
                    doupdate();
                }
                curs_set(0);
                break;
            case 'd': // Delete an item
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

        // Refresh virtual and physical windows
        update_panels();
        doupdate();
    }

    del_panel(todo->panel);
    delwin(todo->win);
    free(todo);
    entry_free(entries);
    endwin();
	return EXIT_SUCCESS;	
}

