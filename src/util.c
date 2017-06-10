/* Functions needed for:
   Generation of nodes in the yacc (.y) file.
   Printing of syntax tree.
*/

#include "globals.h"

/* Function newStmtNode creates a new statement
* node for syntax tree construction
*/
TreeNode * newStmtNode(StmtKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
	t->sibling = NULL;
	t->child[0] = NULL;
	t->child[1] = NULL;
	t->nodekind = StmtK;
	t->kind.stmt = kind;
	//t->lineno = lineno;
	t->attr.name = '\0';
	return t;
}

/* Function newExpNode creates a new expression
* node for syntax tree construction
*/
TreeNode * newExpNode(ExpKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
	t->sibling = NULL;
	t->child[0] = NULL;
	t->child[1] = NULL;
	t->nodekind = ExpK;
	t->kind.exp = kind;
	t->attr.name = '\0';
	//t->lineno = lineno;
	t->type = UnknownT;
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
	for (i = 0; i < indentno; i++)
		printf(" ");
}

/* printindex printing index nicely */
static void printIndex()
{
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
			case FunK:
				printf("Fun\n");
				break;
			case IfK:
				printf("If\n");
				break;
			case RepeatK:
				printf("Repeat\n");
				break;
			case AssignK:
				printf("Assign to: %s\n", "");
				break;
			case ReadK:
				printf("Read: %s\n", "");
				break;
			case WriteK:
				printf("Write\n");
				break;
			case CommentK:
				printf("Comment\n");
				break;
			case ForK:
				printf("For\n");
				break;
			case EndK:
				printf("End\n");
				break;
			case EndlK:
				printf("");
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
			case DecK:
				printf("Const: %f\n", tree->attr.dval);
				break;
			case FunctionK:
				printf("Function: %s\n", tree->attr.name);
				break;
			case IdK:
				printf("Id: %s\n", tree->attr.name);
				break;
			case IndexK:
			{
				if (tree->attr.op!=NULL)
					printf("Index: %s\n", tree->attr.op);
				else
					printf("Index:\n");
			}
			break;
			case ArrayK:
				printf("Array:\n");
				break;
			case ArgsK:
				printf("Arg: %s\n", tree->attr.name);
				break;
			default:
				printf("Unknown ExpNode kind\n");
				break;
			}
		}
		else printf("Unknown node kind\n");
		for (i = 0; i < MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}