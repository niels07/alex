/* util.c: general functions. */

/*
 * Apex calculator
 * by Niels Vanden Eynde
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


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

/* Allocate a chunk of memory of SIZE and intialize, 
   exit with an error message if memory could not be allocated. */
void *
apex_calloc(size_t num, size_t size)
{
    void *ptr = calloc(num, size);

    if (ptr == NULL)
        out_of_memory("calloc", size);

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
