#include "globals.h"

/* Function newStmtNode creates a new statement
* node for syntax tree construction
*/
TreeNode * newStmtNode(StmtKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
		for (i = 0; i<MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		//t->lineno = lineno;
	return t;
}

/* Function newExpNode creates a new expression
* node for syntax tree construction
*/
TreeNode * newExpNode(ExpKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
		for (i = 0; i<MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		//t->lineno = lineno;
		t->type = Void;
	return t;
}

/* Variable indentno is used by printTree to
* store current number of spaces to indent
*/
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
	int i;
	for (i = 0; i<indentno; i++)
		printf(" ");
}

/* printindex printing index nicely */
static void printIndex(Index index)
{
	if (index.l[0])
		printf("[%d", index.l[0]);
	if (index.r[0])
		printf(":%d", index.r[0]);
	if (index.l[1])
		printf(",%d", index.l[1]);
	if (index.r[1])
		printf(":%d", index.r[1]);

	if (index.l[0])
		printf("]");

	printf("\n");
}


/* procedure printTree prints a syntax tree to the
* listing file using indentation to indicate subtrees
*/
void printTree(TreeNode * tree)
{
	int i;
	INDENT;
	while (tree != NULL) {
		printSpaces();
		if (tree->nodekind == StmtK)
		{
			switch (tree->kind.stmt) {
			case IfK:
				printf( "If\n");
				break;
			case RepeatK:
				printf("Repeat\n");
				break;
			case AssignK:
				printf("Assign to: %s", tree->attr.name);
				printIndex(tree->index);
				break;
			case ReadK:
				printf("Read: %s\n", tree->attr.name);
				break;
			case WriteK:
				printf("Write\n");
				break;
			default:
				printf("Unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK)
		{
			switch (tree->kind.exp) {
			case OpK:
				printf("Op: %s\n", tree->attr.op);
				break;
			case ConstK:
				printf("Const: %d\n", tree->attr.val);
				break;
			case IdK:
				printf("Id: %s", tree->attr.name);
				printIndex(tree->index);
				break;
			default:
				printf("Unknown ExpNode kind\n");
				break;
			}
		}
		else printf("Unknown node kind\n");
		for (i = 0; i<MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}

void setindexzero(Index *index)
{
	for (int i = 0; i < 3; i++)
	{
		index->l[i] = 0;
		index->r[i] = 0;
	}
}