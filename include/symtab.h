/* Header file for symbol table. */

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

/* The record in the bucket lists for each variable, including name,
* assigned memory location.
*/
typedef struct BucketListRec
{
	char * name;
	ExpType type; /* memory location for variable */
	TreeNode *m;
	TreeNode *n;
	struct BucketListRec * next;
} *BucketList;

/* Function declarations */
void st_insert(char * name, ExpType type, TreeNode *m, TreeNode *n);

BucketList st_lookup(char * name);

void printSymTab();

#endif