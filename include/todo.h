#ifndef __TODO_H_INCLUDED__
#define __TODO_H_INCLUDED__

#include <curses.h>
#include <panel.h>
#include <form.h>

#define MAX_ENTRY_NAME_SIZE 64

#define ENTRY_INSERT 0
#define ENTRY_DELETE 1
#define ENTRY_MARK 2

typedef struct
{
    char *name; /* Name to be displayed at the top of each window */
    int width;  /* Width of the window in columns */
    int height; /* Height of the window in lines */
    int xpos;   /* The columns that the window should begin at */
    int ypos;   /* The line that the window should begin at */
} WindowMeta;

typedef struct
{
    WindowMeta meta; /* Metadata for the window */
    WINDOW *win;     /* nCurses WINDOW struct */
    PANEL *panel;    /* nCurses PANEL struct */
} MainWindow;

typedef struct
{
    WindowMeta meta; /* Metadata for the window */
    WINDOW *window;  /* nCurses WINDOW struct */
    PANEL *panel;    /* nCurses PANEL struct */
    FORM *form;      /* nCurses FORM struct */
    FIELD **fields;  /* Array of dynamically allocated nCurses FIELD structs */
} FormWindow;

typedef struct Entry_t
{
    int uid;                        /* UID for the entry. Entries are keyed on
                                       this field. */
    char name[MAX_ENTRY_NAME_SIZE]; /* A user-defined name for the entry */
    short isDone;                   /* Flag determning completion status */
    struct Entry_t *next;           /* Pointer to the next entry in the list */
} Entry;

/*********************************************************
 *      BEGIN ENTRY LINKED LIST RELATED FUNCTIONS        *
 *********************************************************/
Entry* entry_create(char *name);
void   entry_destroy(Entry *head);
Entry* entry_find(Entry *head, char *uid);
void   entry_remove(Entry **head, char *uid);
void   entry_mark(Entry **head, char *uid);
void   entry_insert(Entry **head, Entry *e);
void   entry_addUid(Entry **head);
char*  entry_stringize(Entry *e);
void   entry_save(Entry *head, char *filename);
Entry* entry_load(char *filename);

/*********************************************************
 *           BEGIN NCURSES RELATED FUNCTIONS             *
 *********************************************************/

/*********************************************************
 *                BEGIN UTIL FUNCTIONS                   *
 *********************************************************/
char* trim_whitespace(char *str);

#endif
