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
