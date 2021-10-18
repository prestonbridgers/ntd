#include <stdio.h>
#include <stdlib.h>

#include "todo.h"

int main(int argc, char *argv[])
{
	ENTRY *e = entry_create("Do homework");
    printf("Entry's name is: %s\n", e->name);
    if (e->isDone)
        printf("Entry is complete!\n");
    else
        printf("Entry is incomplete...\n");
    free(e);
	return EXIT_SUCCESS;
}

