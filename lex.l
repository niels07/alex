%{
#include <stdlib.h>
#include "y.tab.h"
#include "util.h"
#include "vm.h"
#include "parse.h"

Uint
apex_dostring(const char *string)
{
    YY_BUFFER_STATE buf = yy_scan_string(string); 

    yy_switch_to_buffer(buf);
    yyparse(); 
    yy_delete_buffer(buf);

    if (parse_error)
        return 0;

    apex_execop(); 
    return 1;
}

%}

%%

[0-9]* |
[0-9]+\.[0-9]+  { yylval.f = atof(yytext); return NUMBER; }
[-+*/()%^()]    { return *yytext; }
[ \t]           /* ignore whitespace */ ;
.               { yyerror("Invalid character"); }
%%

int
yywrap(void)
{
    return 1;
}
