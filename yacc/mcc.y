/* This file is a part of the MATLAB to C++ compiler
   Developped by Rasmus Steffensen, May 2017.
*/

%{
#include "globals.h"
#include "util.h"

extern FILE *yyin;
extern int yylex(void);

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
%token <name> COMMENT
%token <dval> DECIMAL
%token <name> NAME
%token SCIENTIFICVAL
%token EOL
%token ATAN ATAN2 COS SIN SQRT SIGN
%token IF END

%type <tree> compstmt statement assignstmt expr term factor var index iexpr array array_inner functions comment

%%

program: 
	  compstmt { savedTree = $1; } 

compstmt:
      /*empty */  { $$ = NULL; }
    | compstmt statement {
		TreeNode * t = $1;
	    if (t != NULL)		  
        {
        	while (t->sibling)
        	t = t->sibling;
        	t->sibling = $2;
			t->sibling->prevsibling = t;
        	$$ = $1;
        }
        else
		{
		/* first node */
		$$ = $2;
		$$->prevsibling = NULL;
		}
		}
;

statement:
	  expr ';'
    | assignstmt ';'
    | comment	
	| EOL { $$=newStmtNode(EndlK); }
;

comment:
	COMMENT { $$=newStmtNode(CommentK); $$->attr.name=$1; }
;

assignstmt:
      var '=' expr {
		$$ = newStmtNode(AssignK);
		$$->child[0] = $1;
		$$->child[1] = $3;
		}
;

index:
	  iexpr { $$ = newExpNode(IndexK); $$->child[0] = $1; }
	| iexpr ':' iexpr { $$ = newExpNode(IndexK); $$->child[0] = $1; $$->child[1] = $3; }
;

iexpr:
	  expr
	| ":" { $$ = newExpNode(IndexAllK); }
;

expr:
	  expr '+' term {
        $$ = newExpNode(OpK);
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.op = "+";
        }
	| expr '-' term {
        $$ = newExpNode(OpK);
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.op = "-";
        }
	| term	{ $$ = $1; }
	| {$$ = NULL; }
;

term:
	  term '*' factor{
	    $$ = newExpNode(OpK);
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.op = "*";
		}
	| term '/' factor {
	    $$ = newExpNode(OpK);
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->attr.op = "/";
		}		
	| factor  { $$ = $1; }		
;

factor :
	 functions	
	| '(' expr ')'  { $$=newExpNode(OpK); $$->attr.op="("; $$->child[0]=$2;  }	
	| var 
	| array
	| NUMBER {
		$$ = newExpNode(ConstK);
    	$$->attr.val = $1; 
    	}
	| DECIMAL {
		$$ = newExpNode(DecK);
    	$$->attr.dval = $1; 
    	}
	| factor '^' factor {
		$$ = newExpNode(OpK);
        $$->child[0] = $1;
        $$->child[1] = $3;
       	$$->attr.op = "^";
		}
	| factor '\'' {
		$$ = newExpNode(OpK);
        $$->child[0] = $1;
       	$$->attr.op = "\'";
		}
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
array:
	  '[' array_inner ']' {
		$$ = newExpNode(ArrayK);
		$$->child[0] = $2;
		}
;	
array_inner:
	  array_inner ';' expr { $$ = newExpNode(IndexK); $$->attr.op = ";"; $$->child[0] = $1; $$->child[1] = $3; }
	| array_inner ',' expr { $$ = newExpNode(IndexK); $$->attr.op = ","; $$->child[0] = $1; $$->child[1] = $3; }
	| expr { $$ = $1;};
;

functions:
	  COS '(' expr ')' { $$ = newExpNode(FunctionK); $$->attr.name = "cos"; $$->child[0] = $3; }
	| SIN '(' expr ')' { $$ = newExpNode(FunctionK); $$->attr.name = "sin"; $$->child[0] = $3; }
	| ATAN2 '(' expr ',' expr ')' { $$ = newExpNode(FunctionK); $$->attr.name = "atan2"; $$->child[0] = $3;  $$->child[1] = $5; }
	| SQRT '(' expr ')' { $$ = newExpNode(FunctionK); $$->attr.name = "sqrt"; $$->child[0] = $3; }
	| ATAN '(' expr ')' { $$ = newExpNode(FunctionK); $$->attr.name = "atan"; $$->child[0] = $3; }
	| SIGN '(' expr ')' { $$ = newExpNode(FunctionK); $$->attr.name = "sign"; $$->child[0] = $3; }
;
%%