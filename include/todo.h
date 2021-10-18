#ifndef __TODO_H_INCLUDED__
#define __TODO_H_INCLUDED__

#define MAX_ENTRY_NAME_SIZE 128

typedef struct ENTRY_T
{
    char name[MAX_ENTRY_NAME_SIZE];
    short isDone;
    struct ENTRY_T *next;
} ENTRY;

ENTRY*  entry_create(char *name, short isDone);
ENTRY*  entry_fromFile(char *filename);
void    entry_free(ENTRY *head);
void    todo_insert(ENTRY **head, ENTRY *e);
void    todo_remove(ENTRY **head, char *name);
ENTRY*  todo_find(ENTRY *head, char*name);
char*   entry_toString(ENTRY *e);
void    entry_toFile(ENTRY *head, char *filename);

#endif
