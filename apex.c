#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

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


