/* cgen.c: Code generation */

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

/* These are used to overide prints to code file
 and instead print to intermediate buffer. This is
 smart if something needs not to be output right away. */
char cmdBuffer[200];
int codeBuffer = 0;

/* prototype for internal recursive code generator */
static void cGen(TreeNode * tree);

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code
* file by traversal of the syntax tree. The
* second parameter (codefile) is the file name
* of the code file, and is used to print the
* file name as a comment in the code file
*/
void codeGen(TreeNode * syntaxTree, char * codefile)
{
	char * s = malloc(strlen(codefile) + 7);
	strcpy(s, "File: ");
	strcat(s, codefile);
	emitComment("MATLAB Compilation to C Code");
	emitComment(s);
	/* generate code for MATLAB program */
	cGen(syntaxTree);
	/* finish */
	emitComment("End of execution.");
}

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode * tree)
{
	TreeNode * p1, *p2, *p3;
	int savedLoc1, savedLoc2, currentLoc;
	int loc;
	switch (tree->kind.stmt) {
	case AssignK:
		emitComment("-> assign");
		emitCode("test");
		/* generate code for rhs */
		cGen(tree->child[0]);
		/* now store value */
		loc = st_lookup(tree->child[0]->attr.name);
		printf("location: %d", loc);
		emitComment("<- assign");
		break; /* assign_k */
	default:
		break;
	}
} /* genStmt */

  /* Procedure genExp generates code at an expression node */
static void genExp(TreeNode * tree)
{
	int loc;
	TreeNode * p1, *p2;
	switch (tree->kind.exp) {

	case ConstK:
		emitComment("-> Const");
		printf("Constant value: %d", tree->attr.val);
		emitComment("<- Const");
		break; /* ConstK */

	case IdK:
		break; /* IdK */

	case OpK:
		p1 = tree->child[0];
		p2 = tree->child[1];
		/* gen code for ac = left arg */
		cGen(p1);
		/* gen code for ac = right operand */
		cGen(p2);
		break; /* OpK */
	default:
		break;
	}
} /* genExp */

  /* Procedure cGen recursively generates code by
  * tree traversal
  */
static void cGen(TreeNode * tree)
{
	if (tree != NULL)
	{
		switch (tree->nodekind) {
		case StmtK:
			genStmt(tree);
			break;
		case ExpK:
			genExp(tree);
			break;
		default:
			break;
		}
		cGen(tree->sibling);
	}
}