#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "todo.h"

/* Trims the leading and trailing whitespace of a given string and returns 
 * the a pointer to it.
 *
 * str - The string to be modified.
 */
char* trim_whitespaces(char *str)
{
    if (str == NULL)
    {
        fprintf(stderr, "trim_whitespace: NULL string\n");
        exit(1);
    }
    char *end;

    //trim leading space
    while(isspace(*str))
        str++;

    if(*str == 0) // all spaces?
        return str;

    // trim trailing space
    end = str + strnlen(str, 128) - 1;

    while(end > str && isspace(*end))
        end--;

    // write new null terminator
    *(end+1) = '\0';

    return str;
}
