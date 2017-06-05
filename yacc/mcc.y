%{
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "util.h"

/* Used to access AST from the main file */
TreeNode * savedTree; 

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
%token EOL

%type <name> var
%type <tree> compstmt statement assignstmt expr term factor
%type <ival> index

%%

program     : compstmt
                 { savedTree = $1;} 

compstmt:        /*empty */  { $$ = NULL; }
                | compstmt statement ';' {
				TreeNode * t = $1;
				   if (t != NULL)		  
                   { while (t->sibling)
                        t = t->sibling;
                     t->sibling = $2;
                     $$ = $1; }
                     else $$ = $2;
				}
				;

statement:
				  expr
                  | assignstmt				  
				  ;

assignstmt:       var '=' expr {
					printf("There was an assignment of %s.\n",$1);
					$$ = newStmtNode(AssignK);
					$$->child[0] = $3;
					$$->attr.name = $1;
					}
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
| factor '^' NUMBER 
;

factor : '(' expr ')'  {  }
| NUMBER { $$ = newExpNode(ConstK);
		   $$->attr.val = $1; }
| var { $$ = newExpNode(IdK);
        $$->attr.name = $1; }
;

var :
				  NAME
				| NAME '[' index ']' {  }
;		

%%