/* This file implements a symbol table for a compiler */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "symtab.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
in hash function  */
#define SHIFT 4

/* the hash function */
static int hash(char * key)
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

/* the hash table */
static BucketList hashTable[SIZE];

/* Procedure st_insert inserts memory locations into the symbol table
* loc = memory location is inserted only the
* first time, otherwise ignored
*/
void st_insert(char * name, ExpType type, TreeNode *m, TreeNode *n)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) /* variable not yet in table */
	{
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->type = type;
		l->next = hashTable[h];
		hashTable[h] = l;
	}
	else /* found in table, do something optional */
	{

	}
} /* st_insert */

  /* Function st_lookup returns the memory
  * location of a variable or -1 if not found
  */
BucketList st_lookup(char * name)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) return (BucketList)-1;
	else return l;
}

/* Procedure printSymTab prints a formatted listing of the symbol 
* table contents to the listing file.
*/
void printSymTab()
{
	int i;
	printf("Variable Name  Type      Array Size\n");
	printf("-------------  --------  ----------\n");
	for (i = 0; i<SIZE; ++i)
	{
		if (hashTable[i] != NULL)
		{
			BucketList l = hashTable[i];
			while (l != NULL)
			{
				printf( "%-14s ", l->name);
				printf("%-9d  ", l->type);
				printf("\n");
				l = l->next;
			}
		}
	}
} /* printSymTab */