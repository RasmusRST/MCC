/* Header file for symbol table. */


/* The record in the bucket lists for each variable, including name,
* assigned memory location.
*/
typedef struct BucketListRec
{
	char * name;
	int memloc; /* memory location for variable */
	struct BucketListRec * next;
} *BucketList;

/* Function declarations */
void st_insert(char * name, int loc);

BucketList st_lookup(char * name);

void printSymTab();