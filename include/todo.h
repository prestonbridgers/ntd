#ifndef __TODO_H_INCLUDED__
#define __TODO_H_INCLUDED__

#include <curses.h>
#include <panel.h>
#include <form.h>

#define MAX_ENTRY_NAME_SIZE 256

#define ENTRY_INSERT 0
#define ENTRY_DELETE 1
#define ENTRY_MARK 2

typedef struct
{
    WINDOW *win;
    PANEL *panel;
    FORM *form;
    FIELD **fields;
    char *name;
    int width;
    int height;
    int xpos;
    int ypos;
} MY_WINDOW;

typedef struct ENTRY_T
{
    int uid;
    char name[MAX_ENTRY_NAME_SIZE];
    short isDone;
    struct ENTRY_T *next;
} ENTRY;

ENTRY*  entry_create(char *name, short isDone);
ENTRY*  entry_fromFile(char *filename);
void    entry_free(ENTRY *head);
void    entry_insert(ENTRY **head, ENTRY *e);
void    entry_remove(ENTRY **head, char *name);
void    entry_mark(ENTRY **head, char *name);
void    entry_addUid(ENTRY **head);
ENTRY*  entry_find(ENTRY *head, char*name);
char*   entry_toString(ENTRY *e);
void    entry_toFile(ENTRY *head, char *filename);

// Related to ncurses
MY_WINDOW *init_todoWindow();
MY_WINDOW *init_addFormWindow();
void draw_window(MY_WINDOW *arg);
void draw_addForm(MY_WINDOW *arg);
void draw_entries(MY_WINDOW *arg, ENTRY *head);
char* trim_whitespaces(char *str);
void form_handle(MY_WINDOW *arg, ENTRY **entries, short action);

#endif
