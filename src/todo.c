#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "todo.h"

ENTRY *entry_create(char *name, short isDone)
{
    if (name == NULL)
    {
        fprintf(stderr, "entry_create: name == null\n");
        exit(1);
    }

    ENTRY *entry = (ENTRY*) malloc(sizeof(*entry));
    strncpy(entry->name, name, MAX_ENTRY_NAME_SIZE);
    entry->isDone = isDone;
    entry->uid = 0;
    entry->next = NULL;

    fprintf(stderr, "Created entry: %s\n", entry->name);

    return entry;
}

ENTRY *entry_fromFile(char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "entry_fromFile: filename == null\n");
        exit(1);
    }
    char line[MAX_ENTRY_NAME_SIZE];
    FILE *file;
    char *entry_name;
    short entry_isDone;
    ENTRY *entries = 0;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File %s does not exist, creating it...\n", filename);
        file = fopen(filename, "w");
        fclose(file);
        return NULL;
    }

    while(fgets(line, MAX_ENTRY_NAME_SIZE, file) != NULL)
    {
        entry_name = strtok(line, "|");
        if (strcmp(strtok(NULL, "|"), "complete\n") == 0)
            entry_isDone = 1;
        else
            entry_isDone = 0;

        fprintf(stderr, "Entry name: %s\n", entry_name);
        fprintf(stderr, "Entry is  : %d\n", entry_isDone);

        entry_insert(&entries, entry_create(entry_name, entry_isDone));
    }
    entry_addUid(&entries);
    fclose(file);
    return entries;
}

void entry_addUid(ENTRY **head)
{
    ENTRY *tmp = *head;
    int count = 0;

    while (tmp != NULL) {
        tmp->uid = count;
        count++;
        tmp = tmp->next;
    }
    return;
}

void entry_toFile(ENTRY *head, char *filename)
{
    if (filename == NULL)
    {
        fprintf(stderr, "entry_toFile: filename == null\n");
        exit(1);
    }

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

ENTRY *entry_find(ENTRY *head, char *uid)
{
    int uidInt = atoi(uid);

    if (uid == NULL)
    {
        fprintf(stderr, "entry_find: name == null\n");
        exit(1);
    }
    // Return null on the empty list
    if (head == NULL)
        return NULL;

    ENTRY *tmp = head;
    for (tmp = head; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->uid == uidInt)
        {
            return tmp;
        }
    }
    return NULL;
}

void entry_mark(ENTRY **head, char *name)
{
    ENTRY *target = entry_find(*head, name);
    fprintf(stderr, "Searching %s found %s\n", name, target->name);
    if (target == NULL)
        return;
    target->isDone = !(target->isDone);
    return;
}

void entry_insert(ENTRY **head, ENTRY *e)
{
    if (e == NULL)
    {
        fprintf(stderr, "entry_insert: e == null\n");
        exit(1);
    }
    if (head == NULL)
    {
        fprintf(stderr, "entry_insert: head == null\n");
        exit(1);
    }

    e->next = *head;
    *head = e;
}

void entry_remove(ENTRY **head, char *uid)
{
    if (uid == NULL)
    {
        fprintf(stderr, "entry_remove: name == null\n");
        exit(1);
    }
    if (head == NULL)
    {
        fprintf(stderr, "entry_remove: head == null\n");
        exit(1);
    }
    ENTRY *prev = NULL;
    ENTRY *target = *head;

    int uidInt = atoi(uid);

    if (target->uid == uidInt)
    {
        *head = (*head)->next;
        free(target);
        return;
    }

    while (target != NULL)
    {
        if (target->uid == uidInt)
        {
            prev->next = target->next;
            free(target);
            return;
        }

        prev = target;
        target = target->next;
    }
    return;
}

char *entry_toString(ENTRY *e)
{
    if (e == NULL)
    {
        fprintf(stderr, "entry_toString: e == NULL\n");
        exit(1);
    }
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
    if (head == NULL)
    {
        return;
    }
    ENTRY *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    return;
}
