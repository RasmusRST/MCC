%{
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "util.h"

extern FILE *yyin;

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

%type <tree> compstmt statement assignstmt expr term factor var index

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
					$$ = newStmtNode(AssignK);
					$$->child[0] = $1;
					$$->child[1] = $3;
					}
;

index:
				  expr { $$ = newExpNode(IndexK); $$->child[0] = $1; }
				| expr ':' expr { $$ = newExpNode(IndexK); $$->child[0] = $1; $$->child[1] = $3; }
;

expr : expr '+' term
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = "+";
                 }
| term          { $$ = $1; }
;

term : term '*' factor
		{
		        $$ = newExpNode(OpK);
                $$->child[0] = $1;
                $$->child[1] = $3;
                $$->attr.op = "*";
		}
| factor  { $$ = $1; }
| factor '^' factor
		{
		        $$ = newExpNode(OpK);
                $$->child[0] = $1;
                $$->child[1] = $3;
                $$->attr.op = "^";
		}
;

factor : '(' expr ')'  { $$ = $2; }
| NUMBER { $$ = newExpNode(ConstK);
		   $$->attr.val = $1; }
| var 
;

var :
				  NAME {
				$$ = newExpNode(IdK);
				$$->attr.name = $1;
				  }
				| NAME '(' index ')' { 
				$$ = newExpNode(IdK);
				$$->attr.name = $1;
				$$->child[0] = $3;
				$$->indexed = 1;
				}
				| NAME '(' index ',' index ')' { 
				$$ = newExpNode(IdK);
				$$->attr.name = $1;
				$$->child[0] = $3;
				$$->child[1] = $5;
				$$->indexed = 2;
				}
;		

%%