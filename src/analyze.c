/* analyze.c: Functions for analyzing an abstract syntax tree */
 
#include "globals.h"
#include "symtab.h"
#include "analyze.h"

 /* counter for variable memory locations */
static int location = 0;

/* Procedure traverse is a generic recursive syntax tree traversal routine:
* it applies preProc in preorder and postProc in postorder to tree pointed to by t
*/
static void traverse(TreeNode * t,
	void(*preProc) (TreeNode *),
	void(*postProc) (TreeNode *))
{
	if (t != NULL)
	{
		preProc(t);
		{ int i;
		for (i = 0; i < MAXCHILDREN; i++)
			traverse(t->child[i], preProc, postProc);
		}
		postProc(t);
		traverse(t->sibling, preProc, postProc);
	}
}

/* nullProc is a do-nothing procedure to generate preorder-only or postorder-only
* traversals from traverse
*/
static void nullProc(TreeNode * t)
{
	if (t == NULL) return;
	else return;
}

/* Procedure insertNode inserts identifiers stored in t into
* the symbol table
*/
static void insertNode(TreeNode * t)
{
	switch (t->nodekind)
	{
	case StmtK:
		switch (t->kind.stmt)
		{
		case AssignK:
			if (st_lookup(t->attr.name) == -1)
				/* not yet in table, so treat as new definition */
				st_insert(t->attr.name, 10);
			break;
		default:
			break;
		}
	default:
		break;
	}
}

/* Function buildSymtab constructs the symbol
* table by preorder traversal of the syntax tree
*/
void buildSymtab(TreeNode * syntaxTree)
{
	traverse(syntaxTree, insertNode, nullProc);
}