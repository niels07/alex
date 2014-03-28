%{
#include "parse.h"
#include "apex.h"
#include "vm.h"
#include "util.h"

Uint parse_error = 0;

void 
yyerror(char *message)
{
    apex_error("%s: unexpeced '%s'", message, yytext);
    parse_error = 1;
}

%}

%union {
    float f;
};

%token <f> NUMBER

%left '+' '-'
%left '*' '/' '^'
%nonassoc UMINUS

%%

program: /* empty */
       | expr { apex_addop(OP_HALT, 0); }
       ;

expr: NUMBER        { apex_addop(OP_NUM, $1);  }
    | expr '+' expr { apex_addop(OP_ADD, 0);   }
    | expr '-' expr { apex_addop(OP_SUB, 0);   }
    | expr '*' expr { apex_addop(OP_MULT, 0);  }
    | expr '/' expr { apex_addop(OP_DIV, 0);   }
    | expr '^' expr { apex_addop(OP_POW, 0);   }
    | '('           { apex_addop(OP_LPAR, 0);  }
       expr ')'     { apex_addop(OP_RPAR, 0);  }
    ;
