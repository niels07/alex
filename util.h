#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#define NEW(x) apex_malloc(sizeof(x))

typedef unsigned int Uint;

/* Allocate a chunk of memory of SIZE, exit with an error
   message if memory could not be allocated. */
extern void *apex_malloc(size_t /* size */);

/* Allocate a chunk of memory of SIZE and intialize, 
   exit with an error message if memory could not be allocated. */
extern void *apex_calloc(size_t /* num */, size_t /* size */);

/* Reallocate a chunk of memory of SIZE, free original 
   memory and exit if memory could not be allocated. 
   If M is NULL, just return apex_malloc() instead. */
extern void *apex_realloc(void * /* m */, size_t /* size */);

/* Return a copy of a string. */
extern char *savestr(const char * /*string */);

#endif /* UTIL_H */
