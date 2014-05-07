#ifndef TABLE_H
#define TABLE_H

#include "util.h"

/* Symbol for storing variables. */
typedef struct symbol {
    /* Identifier. */
    char *name;

    /* Offset of data array. */
    Uint offset;

    /* Next item in table. */
    struct symbol *next;
} Symbol;

extern Symbol *apex_addsymbol(const char * /* name */);
extern Symbol *apex_getsymbol(const char * /* name */);
extern void apex_freetable(void);

#endif /* TABLE_H */
