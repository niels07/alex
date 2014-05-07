%{
#include <stdlib.h>
#include "y.tab.h"
#include "util.h"
#include "vm.h"
#include "parse.h"

Uint
apex_parse(void)
{
    apex_reset();
    return !yyparse();
}

Uint
apex_dostring(const char *string)
{
    YY_BUFFER_STATE buf = yy_scan_string(string); 

    yy_switch_to_buffer(buf);
    apex_parse(); 
    yy_delete_buffer(buf);

    if (parse_error)
        return 0;

    apex_execop(); 
    return 1;
}

%}

%%

[0-9]* |
[0-9]+\.[0-9]+       { yylval.f = atof(yytext); return NUMBER;   }
[-+*/()%^()=;]       { return *yytext;                           }
"print"              { return PRINT;                             }
[a-zA-Z][a-zA-Z0-9]* { yylval.id = savestr(yytext); return ID;   }
[ \t\n]              /* ignore whitespace and newlines */ ;
.                    { yyerror("Invalid character");             }
%%

int
yywrap(void)
{
    return 1;
}
