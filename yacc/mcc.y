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
	varHandler var;
	Index index;
}

%token <ival> NUMBER
%token <string> WORD
%token <dval> DECIMAL
%token <name> NAME
%token SCIENTIFICVAL
%token EOL

%type <var> var
%type <tree> compstmt statement assignstmt expr term factor
%type <index> index

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
					$$->child[0] = $3;
					$$->attr.name = $1.name;
					$$->index = $1.index;
					}
;

index:
				  NUMBER { setindexzero(&$$); $$.l[0] = $1; }
				| NUMBER ',' NUMBER { setindexzero(&$$); $$.l[0] = $1; $$.l[1] = $3; }
				| NUMBER ':' NUMBER { setindexzero(&$$); $$.l[0] = $1; $$.r[0] = $3; }
				| NUMBER ':' NUMBER ',' NUMBER { setindexzero(&$$); $$.l[0] = $1; $$.r[0] = $3; $$.l[1] = $5; }
				| NUMBER ':' NUMBER ',' NUMBER ':' NUMBER { setindexzero(&$$); $$.l[0] = $1; $$.r[0] = $3; $$.l[1] = $5; $$.r[1] = $7; }
				| NUMBER ',' NUMBER ':' NUMBER { setindexzero(&$$); $$.l[0] = $1; $$.l[1] = $3; $$.r[1] = $5; }
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

factor : '(' expr ')'  {  }
| NUMBER { $$ = newExpNode(ConstK);
		   $$->attr.val = $1; }
| var { $$ = newExpNode(IdK);
        $$->attr.name = $1.name;
		$$->index = $1.index;	
		}
;

var :
				  NAME { $$.name = $1; setindexzero(&$$.index); }
				| NAME '(' index ')' { 
				$$.name = $1;
				$$.index = $3;
				}
;		

%%