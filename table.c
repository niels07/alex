/* table.c: symbol table for storing data. */

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
#include "table.h"

/* Table with all symbols. */
static Symbol **symtable = NULL;

/* Allocated size of the table. */
static size_t symtable_size = 255;

/* Index of the symbol. */
static Uint sym_index = 0;

static Uint
getkey(const char *string)
{
    Uint value = 0;

    while (*string != '\0')
        value = ((value << 5) + value) + *string++;
    return value % symtable_size;
}

static Symbol *
newsym(const char *name)
{
    Symbol *symbol = NEW(Symbol);

    symbol->name = savestr(name);
    symbol->offset = sym_index++;
    symbol->next = NULL;

    return symbol;
}

static void
freesym(Symbol *symbol)
{
    free(symbol->name);
    free(symbol);
}

Symbol *
apex_addsymbol(const char *name)
{
    Symbol *symbol;
    Uint key;
    
    key = getkey(name);
    
    if (symtable == NULL) {
        symtable = apex_calloc(symtable_size, sizeof(Symbol *));
        symbol = newsym(name);
        symbol->next = symtable[key];
        symtable[key] = symbol;
        return symbol;
    }
    
    symbol = symtable[key];

    while (symbol != NULL && symbol->next != NULL) {
        if (strcmp(symbol->name, name) == 0) 
            return symbol;
        symbol = symbol->next;
    }

    symbol = newsym(name);
    symbol->next = symtable[key];
    symtable[key] = symbol;

    return symbol;
}

Symbol *
apex_getsymbol(const char *name)
{
    Uint key;
    Symbol *symbol;

    if (symtable == NULL)
        return NULL;

    key = getkey(name);
    symbol = symtable[key];

    while (symbol != NULL) {
        if (strcmp(symbol->name, name) == 0)
            break;
        symbol = symbol->next;
    }

    return symbol;
}

/* Free the symtable. */
void
apex_freetable(void)
{
    Uint i = 0;
    Symbol *symbol, *prev;
    
    while (i < symtable_size) {
        symbol = symtable[i++];
        while (symbol != NULL) {
            prev = symbol;
            symbol = symbol->next;
            freesym(prev);
        }
    }
    free(symtable);
}
