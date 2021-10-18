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

void entry_toFile(ENTRY *head, char *filename)
{
    FILE *file = fopen(filename, "w");
    ENTRY *tmp = head;

    while (tmp != NULL)
    {
        char *entry_str = entry_toString(tmp);
        fprintf(stderr, "Writing \"%s\" to file\n", entry_str);
        fwrite(entry_str, 1, strlen(entry_str), file);
        fputc('\n', file);
        fflush(file);
        free(entry_str);
        tmp = tmp->next;
    }

    fclose(file);
    return;
}

ENTRY *todo_find(ENTRY *head, char *name)
{
    // Return null on the empty list
    if (head == NULL)
        return NULL;

    ENTRY *tmp = head;
    for (tmp = head; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(tmp->name, name))
        {
            return tmp;
        }
    }
    return NULL;
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

    while (target != NULL)
    {
        if (strcmp(target->name, name) == 0)
        {
            prev->next = target->next;
            free(target);
            return;
        }

        prev = target;
        target = target->next;
    }
    fprintf(stderr, "Removing %s (found %s)\n", name, target->name);
    return;
}

char *entry_toString(ENTRY *e)
{
    char *str = (char*) malloc(1024 * sizeof(*str));
    if (e == NULL)
        return "";

    strncpy(str, e->name, MAX_ENTRY_NAME_SIZE);
    strcat(str, "|");
    if (e->isDone)
        strcat(str, "complete");
    else
        strcat(str, "incomplete");
    return str;
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
