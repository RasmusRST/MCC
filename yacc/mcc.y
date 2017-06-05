%{
#include <stdio.h>
#include <string.h>

int yydebug=0;

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	return 1;
}

%}

%union 
{
	int ival; // integer values for constants
	double dval; // double values for constants
	char *string; // string values for constants
	char *name;	 // name of function or variable
}

%token <ival> NUMBER
%token <string> WORD
%token <dval> DECIMAL
%token <name> NAME
%token SCIENTIFICVAL

%type <ival> term factor expr


%%

compstmts:        /*empty */
                | compstmts statements ';'
				| compstmts statements
				;

statements:
				  expr
                  | assignstmt				  
				  ;

assignstmt:       NAME '=' expr { printf("There was an assignment of %s to %i\n at line %i",$1,$3,2); }
;

expr : expr '+' term { $$ = $1 + $3; }
| term
;

term : term '*' factor { $$ = $1 * $3; }
| factor
;

factor : '(' expr ')' { $$ = $2; }
| NUMBER
;