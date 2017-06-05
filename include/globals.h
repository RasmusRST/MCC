#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef enum
/* book-keeping tokens */
{
	test,test1
} TokenType;

typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfK,RepeatK,AssignK,ReadK,WriteK} StmtKind;
typedef enum {OpK,ConstK,IdK,IndexK} ExpKind;

/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean} ExpType;

// used to specify index of arrays
typedef struct indexhandler
{
	// used like a(l[0]:r[0],l[1]:r[1],l[2]:r[2]) = zeros(l2-l1,r2-r1);
	int l[3];
	int r[3];
} Index;

#define MAXCHILDREN 3

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;} kind;
     union { TokenType op;
             int val;
             char * name;
			 Index index;
			} attr;
     ExpType type; /* for type checking of exps */
   } TreeNode;

// used in parser to pass variable name and node
typedef struct varhandler
{
	char *name;
	TreeNode * node;
} varHandler;

#endif