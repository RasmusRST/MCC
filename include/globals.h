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
typedef enum {FunK, IfK, ForK, EndK,RepeatK,AssignK,ReadK,WriteK, CommentK, EndlK} StmtKind;
typedef enum {OpK,ConstK, DecK,IdK,IndexK,IndexAllK, ArrayK, FunctionK, ArgsK} ExpKind;

/* ExpType is used for type checking */
typedef enum {UnknownT,scalarT,vectorT,matrixT} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
	 struct treeNode * prevsibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;} kind;
     union { char *op;
             int val;
			 double dval;
             char * name;			 
			} attr;
	 int indexed;
	 int m; /* rows */
	 int n; /* columns */
	 char *lb; /* buffers to fill up with code if not going to code file. */
	 char *rb; /* lb for left child, rb for right child, see codebuffer and cmdBuffer */
	 ExpType type; /* for type checking of exps */
   } TreeNode;

#endif