

#include <stdio.h>
#include "globals.h"
#include "util.h"
#include "symtab.h"

#include "y.tab.h"

extern TreeNode * savedTree;

/* Used to pass a string to yacc */
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

/* allocate global variables */
int lineno = 0;
FILE *yyin; /* File to yacc / lex */
FILE * outCode; /* C output code */

int main(int argc, char * argv[]){
	/* Print copyrights */
	printf("MATLAB to C compiler (MCC) 0.1\n");
	printf("Copyright (C) 2017 Rasmus Steffensen\n");
	printf("This is free software; There is NO warranty;\n");
	printf("not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n"); 
	
	/* Open the file */
	char pgm[120]; /* source code file name */
	if (argc < 2)
	{
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	strcpy(pgm, argv[1]);
	yyin = fopen(pgm, "r");
	if (yyin == NULL)
	{
		fprintf(stderr, "File %s not found\n", pgm);
		exit(1);
	}

	if (argc != 3)
	{
		outCode = fopen("out.c", "w+");
	}
	else
	{
		outCode = fopen(argv[2], "w+");
	}

	yyparse();
    
	printf("AST:\n");
	printTree(savedTree);
	
	buildSymtab(savedTree);
	printf("\nSymbol table:\n\n");
	printSymTab();
	printf("\n\n");

	codeGen(savedTree, outCode);

    return 0;
}