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
    unsigned int i;
};

%token <f> NUMBER
%token <id> ID
%token PRINT 
%token IF ELSE WHILE
%token EQ NE

%type <i> if_cond if_cmds else else_cmds

%left '+' '-'
%left '*' '/' '^'
%nonassoc UMINUS
%nonassoc LT GT GE LE
%nonassoc EQ NE


%%

program: commands
       ;

commands: /* empty */
        | commands command
        ;

command     : expr              { apex_addop(OP_HALT, 0);  }
            | declare ';'
            | PRINT expr ';'    { apex_addop(OP_PRINT, 0); }
            | statement
            ;

declare     : ID '=' expr       { apex_setvar(OP_STORE, $1); }
            ;

if_cond     : IF '(' expr ')'   { $$ = apex_nextpos(); }
            ;

if_cmds     : '{' commands '}'  { $$ = apex_nextpos(); }
            ;

else        : ELSE              { $$ = apex_getpos(); }
            ;

else_cmds   : '{' commands '}'  { $$ = apex_getpos(); }
            ;

statement:  if_cond if_cmds else else_cmds 
                                { 
                                    apex_setop($1, OP_JMP_FALSE, $3);
                                    apex_setop($2, OP_JMP_TRUE,  $4); 
                                }
            | WHILE '(' expr ')' '{' commands '}'
            ;

expr: NUMBER                    { apex_addop(OP_NUM, $1);  }
            | ID                { getvar($1);              }
            | expr '+' expr     { apex_addop(OP_ADD,  0);  }
            | expr '-' expr     { apex_addop(OP_SUB,  0);  }
            | expr '*' expr     { apex_addop(OP_MULT, 0);  }
            | expr '/' expr     { apex_addop(OP_DIV,  0);  }
            | expr '^' expr     { apex_addop(OP_POW,  0);  }
            | expr EQ  expr     { apex_addop(OP_EQ,   0);  }
            | expr NE  expr     { apex_addop(OP_EQ,   0);  }
            | expr LT  expr     { apex_addop(OP_EQ,   0);  }
            | expr GT  expr     { apex_addop(OP_EQ,   0);  }
            | expr LE  expr     { apex_addop(OP_EQ,   0);  }
            | expr GE  expr     { apex_addop(OP_EQ,   0);  }
            | '('               { apex_addop(OP_LPAR, 0);  }
              expr ')'          { apex_addop(OP_RPAR, 0);  }
            ;
