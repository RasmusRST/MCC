/* cgen.c: Code generation */

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

/* These are used to overide prints to code file
 and instead print to intermediate buffer. This is
 smart if something needs not to be output right away. */
char *cmdBuffer;
int codeBuffer = 0;

/* prototype for internal recursive code generator */
static void cGen(TreeNode * tree);

/* Procedure codeGen generates code to a code file by traversal of the syntax
   tree. The second parameter (codefile) is the file name of the code file,
   and is used to print the file name as a comment in the code file
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
		cGen(tree->child[0]);
		emitCode(" = ");
		/* generate code for rhs */
		cGen(tree->child[1]);
		emitCode(";\n");
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
		emitCode("%d", tree->attr.val);
		break; /* ConstK */

	case IdK:
	{
		TreeNode *i1, *i2, *i3, *i4, *idx1, *idx2;
		i1 = NULL; i2 = NULL; i3 = NULL; i4 = NULL; idx1 = NULL; idx2 = NULL;
		idx1 = tree->child[0];
		idx2 = tree->child[1];
		/* Names to help nicefy code look */
		if (idx1 != NULL)
		{
			i1 = idx1->child[0]; /* (x:.,.:.) */
			i2 = idx1->child[1]; /* (.:x,.:.) */
		}
		if (idx2 != NULL)
		{
			i3 = idx2->child[0]; /* (.:.,x:.) */
			i4 = idx2->child[1]; /* (.:.,.:x) */
		}

		if (idx1 == NULL)
		{ /* Not indexed */
			emitCode("%s", tree->attr.name);
		}
		else
		{ /* Indexed */
			/* Issue here is to handle indices correctly.
			   The idea is that the following code generation
			   will be generated in the buffers lb and rb of idx1
			   and idx2. This buffered code will be inserted in the
			   index of the output C code.
			*/

			// turn on code to buffers.
			char *codeBuffer_old = codeBuffer;
			char *cmdBuffer_old = cmdBuffer;
			codeBuffer = 1;

			// Get code for indices
			idx1->lb = (char *)malloc(sizeof(char) * 50);
			idx1->rb = (char *)malloc(sizeof(char) * 50);
			idx1->lb[0] = '\0';
			idx1->rb[0] = '\0';
			cGen(idx1);

			if (idx2 != NULL)
			{ /* there is also a rhs (.,x) */
				idx2->lb = (char *)malloc(sizeof(char) * 50);
				idx2->rb = (char *)malloc(sizeof(char) * 50);
				idx2->lb[0] = '\0';
				idx2->rb[0] = '\0';
				cGen(idx2);
			}

			// Restore code buffers
			codeBuffer = codeBuffer_old;
			cmdBuffer = cmdBuffer_old;

			// Start code generation of the indices.
			emitCode(tree->attr.name);

			/* four cases of how many indices there is */
			if (i2 == NULL && i3 == NULL)
			{ /* (x) */
				emitCode("(%s)", idx1->lb);
			}
			if (i2 != NULL && i3 == NULL) 
			{ /* (x:x) */
				if (i1->kind.exp == ConstK && i2->kind.exp == ConstK)
					emitCode(".block<%d,%d>(%d,%d)", i1->attr.val - 1, 0, i2->attr.val - i1->attr.val + 1, 1);
				else
					emitCode(".block<%s-1,0>(%s-%s+1,1)", idx1->lb, idx1->rb, idx1->lb);
			}
			if (i2 != NULL && i4 != NULL)
			{  /* (x:x,x:x) */
				/* Collect which values to caluclate and which to substitute.
				   This is because in constant cases for example, the values
				   2+3 should be calculated to 5 for the output.
				*/
				if (i1->kind.exp == ConstK && i2->kind.exp == ConstK &&
					i3->kind.exp == ConstK && i4->kind.exp == ConstK)
					emitCode(".block<%d,%d>(%d,%d)", i1->attr.val - 1, i3->attr.val - 1, i2->attr.val - i1->attr.val + 1, i4->attr.val - i3->attr.val + 1);
				else if (i1->kind.exp == IdK && i2->kind.exp == IdK &&
					i3->kind.exp == IdK && i4->kind.exp == IdK)
					emitCode(".block<%s-1,%s-1>(%s-%s+1,%s-%s+1)", idx1->lb, idx2->lb, idx1->rb, idx1->lb, idx2->rb, idx2->lb);
				else
					emitCode(".block<%s-1,%s-1>(%s-%s+1,%s-%s+1)", idx1->lb, idx2->lb, idx1->rb, idx1->lb, idx2->rb, idx2->lb); // the same as above, add all cases.
			}
		} /* indexed */
	}
	break; /* IdK */
	case IndexK:
	{
		cmdBuffer = tree->lb;
		cGen(tree->child[0]);
		if (tree->child[1] != NULL)
		{
			cmdBuffer = tree->rb;
			cGen(tree->child[1]);
		}
	}
	break;
	case OpK:
		p1 = tree->child[0];
		p2 = tree->child[1];
		/* gen code for ac = left arg */
		cGen(p1);		
		emitCode(" %s ", tree->attr.op);
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