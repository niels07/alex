#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include "util.h"

extern void yyerror(char * /* message */);
extern char *yytext;
extern int yylex(void);
extern int fileno(FILE *);
extern int yyparse(void);
extern Uint apex_dostring(const char * /*string */);
extern Uint parse_error;

#endif /* PARSE_H */
