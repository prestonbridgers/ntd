#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "todo.h"

ENTRY *entry_create(char *name)
{
    ENTRY *entry = (ENTRY*) malloc(sizeof(*entry));
    strncpy(entry->name, name, MAX_ENTRY_NAME_SIZE);
    entry->isDone = 0;
    entry->next = NULL;
    return entry;
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
