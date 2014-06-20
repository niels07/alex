/* apex.c: apex general functions. */

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
#include <stdarg.h>
#include <stdlib.h>
#include "apex.h"
#include "util.h"
#include "parse.h"
#include "table.h"

/* Create and print an error message for apex based on
   FORMAT and following arguments. */
void
apex_error(const char *format, ...)
{
    va_list vl;

    va_start(vl, format);
    fprintf(stderr, "apex: ");
    vfprintf(stderr, format, vl);
    fputc('\n', stderr);
    va_end(vl);
}

Uint 
apex_loadfile(const char *path)
{
    yyin = fopen(path, "r");

    if (yyin == NULL) {
        apex_error("failed to read '%s'", path);
        return 0;
    }

    return 1;
}

Uint
apex_dofile(const char *path)
{
    Uint ret;
    if (!apex_loadfile(path))
        return 0;

    ret = apex_parse();

    fclose(yyin);
    apex_execop();
    return ret;
}

void
apex_end(void)
{
    apex_freetable();
}
