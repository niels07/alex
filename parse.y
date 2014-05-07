%{
#include "parse.h"
#include "apex.h"
#include "table.h"
#include "vm.h"
#include "util.h"

Uint parse_error = 0;

void 
yyerror(char *message)
{
    apex_error("%s: unexpeced '%s'", message, yytext);
    parse_error = 1;
}

static void
getvar(const char *name)
{
    Symbol *symbol = apex_getsymbol(name);

    if (symbol == NULL) {
        apex_error("undefined variable: '%s'", name);
        return;
    }

    apex_addop(OP_VAR, symbol->offset);
}
%}

%union {
    float f;
    char *id;
};

%token <f> NUMBER
%token <id> ID
%token PRINT

%left '+' '-'
%left '*' '/' '^'
%nonassoc UMINUS

%%

program: commands
       ;

commands: /* empty */
        | commands command ';'
        ;

command: expr       { apex_addop(OP_HALT, 0);  }
       | declare 
       | PRINT expr { apex_addop(OP_PRINT, 0); }
       ;

declare: ID '=' expr { apex_setvar(OP_STORE, $1); }
       ;

expr: NUMBER        { apex_addop(OP_NUM, $1);  }
    | ID            { getvar($1);              }
    | expr '+' expr { apex_addop(OP_ADD, 0);   }
    | expr '-' expr { apex_addop(OP_SUB, 0);   }
    | expr '*' expr { apex_addop(OP_MULT, 0);  }
    | expr '/' expr { apex_addop(OP_DIV, 0);   }
    | expr '^' expr { apex_addop(OP_POW, 0);   }
    | '('           { apex_addop(OP_LPAR, 0);  }
       expr ')'     { apex_addop(OP_RPAR, 0);  }
    ;
