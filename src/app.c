#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <curses.h>
#include <panel.h>
#include <form.h>
#include <string.h>

#include "todo.h"

/* Initializes the curses library.
 */
void curs_init()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    // Starting colors and initializing color pairs
    if (has_colors()) {
        start_color();
    }
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    return;
}

int main(int argc, char *argv[])
{
    Entry *entries;
    MainWindow *win_main;
    FormWindow *form_add; // The window that stores the add form
    FormWindow *form_del; // The window that stores the add form
    FormWindow *form_mark; // The window that stores the add form

    char *title_main  = "NTD v0.1";
    char *title_add   = "Add an entry";
    char *title_del   = "Delete an entry";
    char *title_mark  = "Mark an entry";

    // TODO: add this to window_main_create() and draw it in window_main_draw()
    char *footer_main = "'a' and 'd' to add/delete an entry. 'm' to toggle an entry.";

    curs_init();

    // Initializing the windows
    form_mark = window_form_create(title_mark);
    form_del  = window_form_create(title_del);
    form_add  = window_form_create(title_add);
    win_main  = window_main_create(title_main);
    update_panels();

    // Fabricating todo list entries
    entries = entry_load("entries.txt");

    // Printing the title and footer
    mvprintw(LINES - 1, 1, footer_main);
    mvchgat(LINES - 1, 0, -1, A_BOLD, 1, NULL);

    // Drawing the form windows
    window_form_draw(form_add);
    window_form_draw(form_del);
    window_form_draw(form_mark);

    // Drawing the main window
    // TODO: Add a entry_list_stringize() function
    char *tmp = malloc(1024);
    window_main_draw(win_main, entry_stringize_all(entries, tmp));
    free(tmp);

    // Updating screen
    update_panels();
    doupdate();

    char c;
    while ((c = getch()) != 'q')
    {
        switch (c)
        {
            case 'a': // Add an item
                window_form_run(form_add, &entries, ENTRY_INSERT);
                break;
            case 'd': // Delete an item
                window_form_run(form_del, &entries, ENTRY_DELETE);
                break;
            case 'm': // Delete an item
                window_form_run(form_mark, &entries, ENTRY_MARK);
                break;
            default:
                break;
        }
        top_panel(win_main->panel);

        // Re-drawing the main window
        // TODO: Add a entry_list_stringize() function
        char *tmp = malloc(1024);
        window_main_draw(win_main, entry_stringize_all(entries, tmp));
        free(tmp);

        // Refresh virtual and physical windows
        update_panels();
        doupdate();
    }

    /* // After user is done modifying entries, save the state of the entries */
    /* // to a file */
    entry_save(entries, "entries.txt");

    // Free things for the form window
    window_form_destroy(form_add);
    window_form_destroy(form_del);
    window_form_destroy(form_mark);

    window_main_destroy(win_main);
    entry_destroy(entries);
    endwin();
	return EXIT_SUCCESS;	
}

