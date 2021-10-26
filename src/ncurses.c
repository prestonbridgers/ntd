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
window_main_create()
{
    MainWindow *todo  = (MainWindow*) malloc(sizeof(*todo));
    todo->meta.name   = "NTD v0.1";
    todo->meta.width  = COLS - 2;
    todo->meta.height = LINES - 2;
    todo->meta.xpos   = (COLS / 2) - (todo->width / 2);
    todo->meta.ypos   = 1;
    todo->win         = newwin(todo->height, todo->width,
                               todo->ypos, todo->xpos);
    todo->panel       = new_panel(todo->win);
    return todo;
}

/* Creates a FormWindow instance and returns a pointer to it.
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
    add_form_win              = (MY_WINDOW*) malloc(sizeof(*add_form_win));
    add_form_win->meta.name   = "Add an Entry";
    add_form_win->meta.width  = form_width + 2;
    add_form_win->meta.height = form_height + 2;
    add_form_win->meta.xpos   = (COLS / 2) - (add_form_win->meta.width / 2);
    add_form_win->meta.ypos   = (LINES / 2) - (add_form_win->meta.height / 2) + 1;
    add_form_win->win         = newwin(add_form_win->meta.height,
                                       add_form_win->meta.width,
                                       add_form_win->meta.ypos,
                                       add_form_win->meta.xpos);
    add_form_win->fields = fields;
    add_form_win->form   = form;
    add_form_win->panel  = new_panel(add_form_win->win);

    keypad(add_form_win->win, TRUE);

    // Setting the window associated with this form
    set_form_win(form, add_form_win->win);
    // Setting the subwindow for this form to a derived window
    set_form_sub(form, derwin(add_form_win->win, form_height, form_width, 1, 1));

    return add_form_win;
}

/* Frees all memory associated with a FormWindow struct.
 *
 * win - Pointer to the WindowForm struct to be freed.
 */
void
window_form_destroy(FormWindow *win)
{
    unpost_form(add_form_window->form);
    free_form(add_form_window->form);
    free_field(add_form_window->fields[0]);
    free(add_form_window->fields);
    delwin(add_form_window->win);
    del_panel(add_form_window->panel);
    free(add_form_window);
    return;
}

/* Frees all memory associated with a FormWindow struct.
 *
 * win - Pointer to the WindowForm struct to be freed.
 */
void
window_main_destroy(FormWindow *win)
{
    del_panel(todo->panel);
    delwin(todo->win);
    free(todo);
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
        fprintf(stderr, "draw_addForm: win == NULL");
        exit(1);
    }

    box(win->window, 0, 0);
    mvwaddstr(win->window, 0,
              (win->meta.width / 2) - (strlen(win->meta.name) / 2),
              win->meta.name);
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
    if (arg == NULL) {
        fprintf(stderr, "draw_entries: arg == NULL");
        exit(1);
    }
    if (s == NULL) {
        fprintf(stderr, "draw_entries: s == NULL");
        exit(1);
    }

    /* int xpos = 1; */
    /* int ypos = 1; */
    /* char entry_str[MAX_ENTRY_NAME_SIZE]; */
    /* char uidBuf[16]; */
    /* ENTRY *cur; */

    /* werase(arg->win); */
    /* for (cur = head; cur != NULL; cur = cur->next) */
    /* { */
    /*     sprintf(uidBuf, "%d) ", cur->uid); */
    /*     strncpy(entry_str, uidBuf, 16); */

    /*     if (cur->isDone) */
    /*         strncat(entry_str, "[x] ", 5); */
    /*     else */
    /*         strncat(entry_str, "[ ] ", 5); */

    /*     strncat(entry_str, cur->name, MAX_ENTRY_NAME_SIZE - 16 - 5); */
    /*     mvwaddstr(arg->win, ypos, xpos, entry_str); */
    /*     ypos++; */
    /* } */

    mvwprintw(win->window, "%s", s);

    box(win->window, 0, 0);
    mvwaddstr(win->window, 0,
              (win->meta.width / 2) - (strlen(win->meta.name) / 2),
              win->meta.name);
    return;
}

void form_handle(MY_WINDOW *arg, ENTRY **entries, short action)
{
    if (arg == NULL) {
        fprintf(stderr, "form_add: arg == NULL\n");
        exit(1);
    }

    if (entries == NULL) {   
        fprintf(stderr, "form_add: entries == NULL\n");
        exit(1);
    }

    top_panel(arg->panel);
    update_panels();
    doupdate();
    curs_set(1);

    form_driver(arg->form, REQ_NEXT_FIELD);
    form_driver(arg->form, REQ_PREV_FIELD);
    // Enter form input loop
    int f;
    int done = 0;
    while (!done)
    {
        f = wgetch(arg->win);
        switch (f)
        {
            case 10: // ENTER
                // sync the field buffer with what's displayed
                form_driver(arg->form, REQ_NEXT_FIELD);
                form_driver(arg->form, REQ_PREV_FIELD);
                // ADD ENTRY TO THE LIST HERE
                //FIELD **fields = form_fields(arg->form);
                char *field_buf = field_buffer(arg->fields[0], 0);
                char *field_buf_trimmed = trim_whitespaces(field_buf);
                if (strcmp(field_buf_trimmed, "") == 0)
                {
                    done = 1;
                    break;
                }
                if (action == ENTRY_DELETE)
                    entry_remove(entries, field_buf_trimmed);
                if (action == ENTRY_INSERT)
                    entry_insert(entries, entry_create(field_buf_trimmed, 0));
                if (action == ENTRY_MARK)
                    entry_mark(entries, field_buf_trimmed);
                entry_addUid(entries);
                form_driver(arg->form, REQ_CLR_FIELD);
                done = 1;
                break;
            case KEY_BACKSPACE:
                form_driver(arg->form, REQ_DEL_PREV);
                break;
            case KEY_LEFT:
                form_driver(arg->form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(arg->form, REQ_NEXT_CHAR);
                break;
            default:
                form_driver(arg->form, f);
                break;
        }
    }
    curs_set(0);
    return;
}
