#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "util.h"
#include "vm.h"
#include "parse.h"

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

int
main(int argc, char *argv[])
{
    float result;
    if (argc == 1)
        return EXIT_SUCCESS;

    if (!apex_dostring(argv[1]))
        return EXIT_FAILURE;

    result = apex_getnumber(apex_gettop());

    printf("%f\n", result);
    return EXIT_SUCCESS;
}
