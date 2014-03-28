#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apex.h"

static void
out_of_memory(const char *name, size_t size)
{
    apex_error("(%s) failed to allocate %d bytes", name, size);
    exit(EXIT_FAILURE);
}

/* Allocate a chunk of memory of SIZE, exit with an error
   message if memory could not be allocated. */
void *
apex_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
        out_of_memory("malloc", size);

    return ptr;
}

/* Reallocate a chunk of memory of SIZE, free original 
   memory and exit if memory could not be allocated. 
   If M is NULL, just return apex_malloc() instead. */
void *
apex_realloc(void *m, size_t size)
{
    void *ptr;

    if (m == NULL)
        return apex_malloc(size);

    ptr = realloc(m, size);

    if (ptr == NULL) {
        free(m);
        out_of_memory("realloc", size);
    }

    return ptr;
}

/* Return a copy of a string. */
char *
savestr(const char *string)
{
    char *copy;
    size_t len = strlen(string);

    copy = apex_malloc(len + 1);
    strncpy(copy, string, len);
    copy[len] = '\0';

    return copy;
}
