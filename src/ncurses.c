#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <panel.h>
#include <form.h>

#include "todo.h"

void form_handle(MY_WINDOW *arg, ENTRY **entries, short action)
{
    if (arg == NULL)
    {
        fprintf(stderr, "form_add: arg == NULL\n");
        exit(1);
    }

    if (entries == NULL)
    {   
        fprintf(stderr, "form_add: entries == NULL\n");
        exit(1);
    }

    top_panel(arg->panel);
    update_panels();
    doupdate();
    curs_set(1);

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
                if (action == ENTRY_DELETE)
                    entry_remove(entries, field_buf_trimmed);
                if (action == ENTRY_INSERT)
                    entry_insert(entries, entry_create(field_buf_trimmed, 0));
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
    curs_set(1);
}

// This is broken I think
char* trim_whitespaces(char *str)
{
    if (str == NULL)
    {
        fprintf(stderr, "trim_whitespace: NULL string\n");
        exit(1);
    }
    char *end;

    //trim leading space
    while(isspace(*str))
        str++;

    if(*str == 0) // all spaces?
        return str;

    // trim trailing space
    end = str + strnlen(str, 128) - 1;

    while(end > str && isspace(*end))
        end--;

    // write new null terminator
    *(end+1) = '\0';

    return str;
}

MY_WINDOW *init_addFormWindow()
{
    MY_WINDOW *add_form_win;
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
    add_form_win         = (MY_WINDOW*) malloc(sizeof(*add_form_win));
    add_form_win->name   = "Add an Entry";
    add_form_win->width  = form_width + 2;
    add_form_win->height = form_height + 2;
    add_form_win->xpos   = (COLS / 2) - (add_form_win->width / 2);
    add_form_win->ypos   = (LINES / 2) - (add_form_win->height / 2) + 1;
    add_form_win->win    = newwin(add_form_win->height, add_form_win->width,
                                  add_form_win->ypos, add_form_win->xpos);
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

void draw_addForm(MY_WINDOW *arg)
{
    if (arg == NULL)
    {
        fprintf(stderr, "draw_addForm: arg == NULL");
        exit(1);
    }
    box(arg->win, 0, 0);
    mvwaddstr(arg->win, 0, (arg->width / 2) - (strlen(arg->name) / 2), arg->name);
    post_form(arg->form);
    return;
}

MY_WINDOW *init_todoWindow()
{
    MY_WINDOW *todo = (MY_WINDOW*) malloc(sizeof(*todo));
    todo->name      = "TODO Items";
    todo->width     = COLS - 2;
    todo->height    = LINES - 2;
    todo->xpos      = (COLS / 2) - (todo->width / 2);
    todo->ypos      = (LINES / 2) - (todo->height / 2) + 1;
    todo->win       = newwin(todo->height, todo->width,
                             todo->ypos, todo->xpos);
    todo->panel     = new_panel(todo->win);
    return todo;
}



void draw_window(MY_WINDOW *arg)
{
    if (arg == NULL)
    {
        fprintf(stderr, "draw_window: arg == NULL");
        exit(1);
    }
    box(arg->win, '|', '-');
    mvwaddstr(arg->win, 0, (arg->width / 2) - (strlen(arg->name) / 2),
              arg->name);
    return;
}

void draw_entries(MY_WINDOW *arg, ENTRY *head)
{
    if (arg == NULL)
    {
        fprintf(stderr, "draw_entries: arg == NULL");
        exit(1);
    }
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
