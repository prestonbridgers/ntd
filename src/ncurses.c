#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <panel.h>
#include <form.h>

#include "todo.h"

/* Creates the MainWindow instance and returns a pointer to it.
 */
MainWindow*
window_main_create(char *name)
{
    MainWindow *todo  = (MainWindow*) malloc(sizeof(*todo));
    strncpy(todo->meta.name, name, MAX_WINDOW_NAME_SIZE);
    todo->meta.width  = COLS - 2;
    todo->meta.height = LINES - 2;
    todo->meta.xpos   = (COLS / 2) - (todo->meta.width / 2);
    todo->meta.ypos   = 1;
    todo->window      = newwin(todo->meta.height, todo->meta.width,
                               todo->meta.ypos, todo->meta.xpos);
    todo->panel       = new_panel(todo->window);
    return todo;
}

/* Creates a FormWindow instance and returns a pointer to it.
 *
 * name - The name that will be displayed along the top border of the window.
 */
FormWindow*
window_form_create(char *name)
{
    FormWindow *add_form_win;
    FIELD **fields;
    FORM *form;
    int form_width;
    int form_height;

    // Initializing the fields
    fields = (FIELD**) malloc(2 * sizeof(*fields));
    fields[0] = new_field(1, 30, 0, 0, 0, 0);
    fields[1] = NULL;

    set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    // Creating the form
    form = new_form(fields);

    // Getting the lines and cols needed for the form
    scale_form(form, &form_height, &form_width);

    // Creating the MY_WINDOW struct
    add_form_win              = (FormWindow*) malloc(sizeof(*add_form_win));
    strncpy(add_form_win->meta.name, name, MAX_WINDOW_NAME_SIZE);
    add_form_win->meta.width  = form_width + 2;
    add_form_win->meta.height = form_height + 2;
    add_form_win->meta.xpos   = (COLS / 2) - (add_form_win->meta.width / 2);
    add_form_win->meta.ypos   = (LINES / 2) - (add_form_win->meta.height / 2) + 1;
    add_form_win->window      = newwin(add_form_win->meta.height,
                                       add_form_win->meta.width,
                                       add_form_win->meta.ypos,
                                       add_form_win->meta.xpos);
    add_form_win->fields = fields;
    add_form_win->form   = form;
    add_form_win->panel  = new_panel(add_form_win->window);

    keypad(add_form_win->window, TRUE);

    // Setting the window associated with this form
    set_form_win(form, add_form_win->window);
    // Setting the subwindow for this form to a derived window
    set_form_sub(form, derwin(add_form_win->window, form_height, form_width, 1, 1));

    return add_form_win;
}

/* Frees all memory associated with a FormWindow struct.
 *
 * win - Pointer to the WindowForm struct to be freed.
 */
void
window_form_destroy(FormWindow *win)
{
    unpost_form(win->form);
    free_form(win->form);
    free_field(win->fields[0]);
    free(win->fields);
    delwin(win->window);
    del_panel(win->panel);
    free(win);
    return;
}

/* Frees all memory associated with a FormWindow struct.
 *
 * win - Pointer to the WindowForm struct to be freed.
 */
void
window_main_destroy(MainWindow *win)
{
    del_panel(win->panel);
    delwin(win->window);
    free(win);
    return;
}

/* Draws the form windows to the nCurses virtual buffer.
 *
 * win - Pointer to the FormWindow struct to be drawn.
 */
void
window_form_draw(FormWindow *win)
{
    if (win == NULL) {
        /* fprintf(stderr, "draw_addForm: win == NULL"); */
        exit(1);
    }

    box(win->window, 0, 0);
    mvwaddstr(win->window, 0,
              (win->meta.width / 2) - (strlen(win->meta.name) / 2),
              win->meta.name);
    mvwchgat(win->window, 0, win->meta.width / 2 - strlen(win->meta.name) / 2, strlen(win->meta.name), A_BOLD, 1, NULL);
    post_form(win->form);
    return;
}

/* Draws a MainWindow struct to nCurses virtual buffer, then prints a string
 * onto that window.
 *
 * win - Pointer to the MainWindow struct to be drawn.
 * s   - The string to be drawn onto win.
 */
void
window_main_draw(MainWindow *win, char *s)
{
    if (win == NULL) {
        /* fprintf(stderr, "draw_entries: arg == NULL"); */
        exit(1);
    }
    if (s == NULL) {
        /* fprintf(stderr, "draw_entries: s == NULL"); */
        exit(1);
    }

    werase(win->window);
    mvwprintw(win->window, 1, 1, "%s", s);
    box(win->window, 0, 0);
    mvwaddstr(win->window, 0,
              (win->meta.width / 2) - (strlen(win->meta.name) / 2),
              win->meta.name);
    mvwchgat(win->window, 0, win->meta.width / 2 - strlen(win->meta.name) / 2, strlen(win->meta.name), A_BOLD, 1, NULL);
    return;
}

/* Handles all the user interaction with the nCurses forms.
 *
 * win - The FormWindow struct that is being used.
 * entries - The list of Entry structs that is being used (mark/add/remove).
 * action - An action define that will be switched on (ENTRY_INSERT,
 *          ENTRY_DELTE, or ENTRY_MARK).
 */
void
window_form_run(FormWindow *win, Entry **entries, short action)
{
    if (win == NULL) {
        /* fprintf(stderr, "form_add: arg == NULL\n"); */
        exit(1);
    }

    if (entries == NULL) {   
        /* fprintf(stderr, "form_add: entries == NULL\n"); */
        exit(1);
    }

    curs_set(1);
    top_panel(win->panel);
    update_panels();
    doupdate();

    // Enter form input loop
    int f;
    int done = 0;
    while (!done) {
        f = wgetch(win->window);
        switch (f) {
            case 10: // ENTER
                // Sync the input buffer
                form_driver(win->form, REQ_NEXT_FIELD);
                form_driver(win->form, REQ_PREV_FIELD);

                //FIELD **fields = form_fields(arg->form);
                char *field_buf = field_buffer(win->fields[0], 0);
                char *field_buf_trimmed = trim_whitespace(field_buf);
                if (strcmp(field_buf_trimmed, "") == 0) {
                    done = 1;
                    break;
                }
                if (action == ENTRY_DELETE) {
                    entry_remove(entries, field_buf_trimmed);
                } else if (action == ENTRY_INSERT) {
                    entry_insert(entries, entry_create(field_buf_trimmed));
                } else if (action == ENTRY_MARK) {
                    entry_mark(entries, field_buf_trimmed);
                }
                entry_addUid(entries);
                form_driver(win->form, REQ_CLR_FIELD);
                done = 1;
                break;
            case KEY_BACKSPACE:
                form_driver(win->form, REQ_DEL_PREV);
                break;
            case KEY_LEFT:
                form_driver(win->form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(win->form, REQ_NEXT_CHAR);
                break;
            default:
                form_driver(win->form, f);
                break;
        }
    }
    curs_set(0);
    return;
}
