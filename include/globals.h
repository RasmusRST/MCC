#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

extern FILE* source;  /* source code text file */
extern FILE* outCode; /* C output code */

typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfK,RepeatK,AssignK,ReadK,WriteK} StmtKind;
typedef enum {OpK,ConstK, DecK,IdK,IndexK,IndexAllK, ArrayK, FunctionK, CommentK} ExpKind;

/* ExpType is used for type checking */
typedef enum {UnknownT,scalarT,vectorT,matrixT} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;} kind;
     union { char *op;
             int val;
			 double dval;
             char * name;			 
			} attr;
	 int indexed;
	 int m;
	 int n;
	 char *lb;
	 char *rb;
	 ExpType type; /* for type checking of exps */
   } TreeNode;

#endif