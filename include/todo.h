#ifndef __TODO_H_INCLUDED__
#define __TODO_H_INCLUDED__

#define MAX_ENTRY_NAME_SIZE 128

typedef struct
{
    char name[MAX_ENTRY_NAME_SIZE];
    short isDone;
} ENTRY;

ENTRY*  entry_create(char *name);

#endif
