#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "todo.h"

ENTRY *entry_create(char *name, short isDone)
{
    ENTRY *entry = (ENTRY*) malloc(sizeof(*entry));
    strncpy(entry->name, name, MAX_ENTRY_NAME_SIZE);
    entry->isDone = isDone;
    entry->next = NULL;
    return entry;
}

ENTRY *entry_fromFile(char *filename)
{
    char line[MAX_ENTRY_NAME_SIZE];
    FILE *file = fopen(filename, "r");
    char *entry_name;
    short entry_isDone;
    ENTRY *entries = 0;

    while(fgets(line, MAX_ENTRY_NAME_SIZE, file) != NULL)
    {
        entry_name = strtok(line, "|");
        if (strcmp(strtok(NULL, "|"), "complete\n") == 0)
            entry_isDone = 1;
        else
            entry_isDone = 0;

        fprintf(stderr, "Entry name: %s\n", entry_name);
        fprintf(stderr, "Entry is  : %d\n", entry_isDone);

        todo_insert(&entries, entry_create(entry_name, entry_isDone));
    }
    fclose(file);
    return entries;
}

void todo_insert(ENTRY **head, ENTRY *e)
{
    e->next = *head;
    *head = e;
}

void todo_remove(ENTRY **head, char *name)
{
    ENTRY *prev;
    ENTRY *target = *head;

    while (strcmp(target->name, name))
    {
        prev = target;
        target = target->next;
    }
    fprintf(stderr, "Removing %s (found %s)\n", name, target->name);

    prev->next = target->next;
    free(target);
    return;
}

void entry_free(ENTRY *head)
{
    ENTRY *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    return;
}
