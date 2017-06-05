%{
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "util.h"

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
	TreeNode * tree;
}

%token <ival> NUMBER
%token <string> WORD
%token <dval> DECIMAL
%token <name> NAME
%token SCIENTIFICVAL

%type <name> var
%type <tree> compstmts statements assignstmt expr term factor
%type <ival> index

%%
compstmts:        /*empty */  {  }
                | compstmts statements ';'
				| compstmts statements
				;

statements:
				  expr
                  | assignstmt				  
				  ;

assignstmt:       var '=' expr { printf("There was an assignment of %s",$1); $$ = newStmtNode(AssignK); }
;

index:
				  NUMBER
				| NUMBER ',' NUMBER
				| NUMBER ';' NUMBER
				| '[' index ']' ';' NUMBER { $$ = 2; }
				| '[' index ']' ',' NUMBER { $$ = 2; }
				| NUMBER ';' '[' index ']' 
				| NUMBER ',' '[' index ']'
;

expr : expr '+' term
| term
;

term : term '*' factor
| factor
| factor '^' NUMBER { $$ = newStmtNode(AssignK); }
;

factor : '(' expr ')'  {  }
| NUMBER { $$ = newStmtNode(AssignK); }
| var { $$ = newStmtNode(AssignK); }
;

var :
				  NAME
				| NAME '[' index ']' {  }
;		