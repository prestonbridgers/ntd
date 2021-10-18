#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "todo.h"

ENTRY *entry_create(char *name)
{
    ENTRY *entry = (ENTRY*) malloc(sizeof(*entry));
    strncpy(entry->name, name, MAX_ENTRY_NAME_SIZE);
    entry->isDone = 0;
    return entry;
}
