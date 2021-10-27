#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "todo.h"

/* Creates an Entry struct on the heap and returns a pointer to it.
 *
 * name - The name field of the entry to be created.
 */
Entry*
entry_create(char *name)
{
    if (name == NULL) {
        fprintf(stderr, "entry_create: name == null\n");
        exit(1);
    }

    Entry *entry = (Entry*) malloc(sizeof(*entry));
    strncpy(entry->name, name, MAX_ENTRY_NAME_SIZE);
    entry->isDone = 0;
    entry->uid = -1;
    entry->next = NULL;

    fprintf(stderr, "Created entry: %s\n", entry->name);

    return entry;
}

/* Generates a list of Entry structs from a file and returns a pointer to it.
 *
 * filename - The name of the file from which to read entries.
 */
Entry*
entry_load(char *filename)
{
    char line[MAX_ENTRY_NAME_SIZE];
    FILE *file;
    char *entry_name;
    Entry *entries = NULL;
    Entry *new;

    if (filename == NULL) {
        fprintf(stderr, "entry_fromFile: filename == null\n");
        exit(1);
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "File \"%s\" does not exist, creating it...\n",
                filename);
        file = fopen(filename, "w");
    }

    while(fgets(line, MAX_ENTRY_NAME_SIZE, file) != NULL)
    {
        entry_name = strtok(line, "|");
        new = entry_create(entry_name);
        if (strcmp(strtok(NULL, "|"), "complete\n") == 0) {
            new->isDone = 1;
        } else {
            new->isDone = 0;
        }

        entry_insert(&entries, new);
    }
    entry_addUid(&entries);
    fclose(file);
    return entries;
}

/* Adds the UIDs to each entry in a list.
 *
 * head - Double pointer to the list of Entry structs to modify.
 */
void
entry_addUid(Entry **head)
{
    Entry *tmp = *head;
    int count = 1;

    while (tmp != NULL) {
        tmp->uid = count;
        count++;
        tmp = tmp->next;
    }
    return;
}

/* Stringizes a list of Entry structs and writes it to a file.
 *
 * head     - The list of Entry structs to write to the file.
 * filename - The name of the file to which to write the entries' string.
 */
void
entry_save(Entry *head, char *filename)
{
    if (filename == NULL) {
        fprintf(stderr, "entry_toFile: filename == null\n");
        exit(1);
    }

    FILE *file = fopen(filename, "w");
    Entry *tmp = head;

    while (tmp != NULL) {
        char *entry_str = (char*) malloc(MAX_ENTRY_NAME_SIZE + 18);
        entry_stringize(entry_str, tmp);
        fwrite(entry_str, 1, strlen(entry_str), file);
        free(entry_str);
        fputc('\n', file);
        fflush(file);
        tmp = tmp->next;
    }

    fclose(file);
    return;
}

/* Finds a specific Entry struct in a list and returns a pointer to it.
 * Returns NULL if the Entry is not found in the list.
 *
 * head - The Entry struct list to search.
 * uid  - The uid associated with the Entry to be searched.
 */
Entry*
entry_find(Entry *head, char *uid)
{
    int uidInt = atoi(uid);
    if (uidInt == 0) { // Error parsing
        return NULL;
    }


    if (uid == NULL) {
        fprintf(stderr, "entry_find: name == null\n");
        exit(1);
    }
    // Return null on the empty list
    if (head == NULL) {
        return NULL;
    }

    Entry *tmp = head;
    for (tmp = head; tmp != NULL; tmp = tmp->next) {
        if (tmp->uid == uidInt) {
            return tmp;
        }
    }
    return NULL;
}

/* Finds an Entry in an Entry struct list and marks it as complete (toggles the
 * isDone flag member of the Entry struct). Does nothing if the Entry cannot
 * be found in the list.
 *
 * head - A double pointer to the list of Entry structs to potentially modify.
 * uid  - The uid associated with the Entry to be searched.
 */
void
entry_mark(Entry **head, char *uid)
{
    Entry *target = entry_find(*head, uid);
    if (target == NULL) {
        return;
    }
    target->isDone = !(target->isDone);
    return;
}

/* Inserts an Entry struct into a list of Entry structs.
 *
 * head - A double pointer to the list of Entry structs to which we're
 *        inserting.
 * e    - The Entry struct to be inserted.
 */
void
entry_insert(Entry **head, Entry *e)
{
    if (e == NULL) {
        fprintf(stderr, "entry_insert: e == null\n");
        exit(1);
    }

    e->next = *head;
    *head = e;
    return;
}

/* Finds then removes an entry from a list of Entries. Does nothing if the list
 * is empty or the entry already does not exist in the list.
 *
 * head - A double pointer to the list of Entries from which we're removing.
 * uid  - The uid associated with the Entry to be searched.
 */
void
entry_remove(Entry **head, char *uid)
{
    if (uid == NULL) {
        fprintf(stderr, "entry_remove: name == null\n");
        exit(1);
    }

    if (head == NULL) {
        return;
    }

    Entry *prev = NULL;
    Entry *target = *head;

    int uidInt = atoi(uid);

    if (target->uid == uidInt) {
        *head = (*head)->next;
        free(target);
        return;
    }

    while (target != NULL) {
        if (target->uid == uidInt) {
            prev->next = target->next;
            free(target);
            return;
        }

        prev = target;
        target = target->next;
    }
    return;
}

/* Takes a single Entry struct and stringizes it reutrning a pointer to the 
 * string.
 *
 * e - The Entry struct to be stringized.
 */
char*
entry_stringize(char *dest, Entry *e)
{
    if (e == NULL) {
        fprintf(stderr, "entry_toString: e == NULL\n");
        exit(1);
    }

    if (e == NULL) {
        return "";
    }

    sprintf(dest, "%s%s", e->name, "|");

    if (e->isDone) {
        strcat(dest, "complete");
    } else {
        strcat(dest, "incomplete");
    }
    return dest;
}

/* Stringizes an entire list of Entry structs.
 * This is called by main to get the string representation of a list of
 * entries to pass to the window_draw_main() function. This and the draw
 * function are not coupled.
 *
 * head    - Pointer to the linked list of Entry structs.
 * dest    - Pointer to a string where the string will be stored.
 *
 * returns - The pointer to the modified string.
 */
char*
entry_stringize_all(Entry *head, char *dest)
{
    char line[1024];
    char done_str[32];
    Entry *cur;

    strcpy(dest, "");

    for (cur = head; cur != NULL; cur = cur->next) {
        if (cur->isDone) {
            strcpy(done_str, "[x]");
        } else {
            strcpy(done_str, "[ ]");
        }

        sprintf(line, "%d) %s %s\n ", cur->uid, done_str, cur->name);
        strcat(dest, line);
    }
    return dest;
}

/* Frees the memory associated with a given Entry struct.
 *
 * head - The Entry struct to free.
 */
void
entry_destroy(Entry *head)
{
    Entry *tmp;

    if (head == NULL) {
        return;
    }

    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    return;
}
