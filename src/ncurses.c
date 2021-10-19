#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <panel.h>
#include <form.h>

#include "todo.h"

// This is broken I think
char* trim_whitespaces(char *str)
{
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
    end++;
    *(end) = '\0';

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
    fields[0] = new_field(1, 30, 1, 1, 0, 0);
    fields[1] = NULL;

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
    todo->height    = LINES - 3;
    todo->xpos      = (COLS / 2) - (todo->width / 2);
    todo->ypos      = (LINES / 2) - (todo->height / 2) + 1;
    todo->win       = newwin(todo->height, todo->width,
                             todo->ypos, todo->xpos);
    todo->panel     = new_panel(todo->win);
    return todo;
}



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
