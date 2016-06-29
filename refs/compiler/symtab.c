#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/*
 * SIZE is the size of the hash table
 */
#define SIZE  211

/*
 * SHIFT is the power of two used as multiplier
 * in hash function
 */
#define SHIFT  4

static int hash(char *key)
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0') {
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

/*
 * the list of line numbers of the source code
 * in which a variable is referenced
 */
typedef struct LineListRec {
	int lineno;
	struct LineListRec *next;
} *LineList;

/*
 * The record in the bucket lists for each
 * variable, including name, assigned
 * memory location, and the list of line
 * numbers in which it appears in the source
 * code
 */
typedef struct BucketListRec {
	char *name;
	LineList lines;
	int memloc;  /* memory location for variable */
	struct BucketListRec *next;
} *BucketList;

/*
 * the hash table
 */
static BucketList hashTable[SIZE];

/*
 * Procedure st_insert inserts line numbers and mmeory
 * locations into the symbol table loc = memory location
 * is inserted only the first time, otherwise ignored
 */
void st_insert(char *name, int lineno, int loc)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while (l != NULL && strcmp(name, l->name) != 0)
		l = l->next;
	if (l == NULL) {
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->lines = (LineList)malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->lines->next = NULL;
		l->next = hashTable[h];
		hashTable[h] = l;
	} else {
		LineList t = l->lines;
		while (t->next != NULL)
			t = t->next;
		t->next = (LineList)malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
}

/*
 * Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup(char *name)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while (l != NULL && strcmp(name, l->name) != 0)
		l = l->next;
	if (l == NULL)
		return -1;
	else
		return l->memloc;
}

/*
 * Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing table
 */
void printSymTab(FILE *listing)
{
	int i;
	fprintf(listing, "Variable Name  Location  Line Numbers\n");
	fprintf(listing, "-------------  --------  ------------\n");
	for (i = 0; i < SIZE; ++i) {
		if (hashTable[i] != NULL) {
			BucketList l = hashTable[i];
			while (l != NULL) {
				fprintf(listing, "%-14s ", t->name);
				fprintf(listing, "%-8d  ", t->memloc);
				while (t != NULL) {
					fprintf(listing, "%4d ", t->lineno);
					t = t->next;
				}
				fprintf(listing, "\n");
				l = l->next;
			}
		}
	}
}
