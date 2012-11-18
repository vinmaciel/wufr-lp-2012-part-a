/*
 * Table.c
 *
 *  Created on: 09/05/2012
 *      Author: Leonardo Pedro Kakitani
 *      		Marcos Vinicius Maciel da Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Table.h"

void createTable(FILE* input, Table* table) {
	int i;
	char name[NAME_LENGTH];

	fscanf(input, "%d", &(table->size));

	table->elem = (char**) malloc((table->size)*sizeof(char*));
	for(i = 0; i < table->size; i++) {
		table->elem[i] = (char*) malloc((strlen(name)+1)*sizeof(char));
		table->elem[i][0] = '\0';
	}

	for(i = 0; i < table->size; i++) {
		getName(input, name);

		if(findIndex(*table, name) >= 0) {
			printf("ERROR: Table element \"%s\" redefined.\n", name);
			fflush(stdout);
			exit(1);
		}

		strcpy(table->elem[i], name);
	}
}

int findIndex(Table table, char* name) {
	int i, node;

	node = 1;
	for(i = 0; i < table.size && table.elem[i] != NULL && node != 0; i++)
		node = strcmp(table.elem[i], name);

	if(node == 0 && i <= table.size) return --i;
	else return -1;
}

void getName(FILE* input, char* newName) {
	int i, quotes;
	char letter;
	char name[NAME_LENGTH];

	quotes = 0;
	for(i = 0; i < NAME_LENGTH; i++) {
		letter = fgetc(input);

		if(letter == EOF) {
			newName[0] = letter;
			return;
		}

		if(quotes == 1) {
			if(letter == '\\') {
				letter = fgetc(input);

				if(letter == '*' || letter == '\\' || letter == '\'')
					name[i] = letter;
				else {
					name[i++] = '\'';
					name[i] = letter;
				}
			}
			else if(letter == '\'') {
				name[i] = '\0';
				i = NAME_LENGTH;
			}
			else
				name[i] = letter;
		}
		else {
			if(letter == '\'') {
				quotes = 1;
				i--;
			}
			else
				i--;
		}
	}

	strcpy(newName, name);
}

int isAcceptState(int indexState, Table stateTable) {
	if(stateTable.elem[indexState][0] == '*' && strlen(stateTable.elem[indexState]) > 1)
		return 1;

	return 0;
}

int isSubMachine(int indexSymbol, Table symbolTable) {
	if(symbolTable.elem[indexSymbol][0] == '<' && symbolTable.elem[indexSymbol][strlen(symbolTable.elem[indexSymbol])-1] == '>')
		return 1;

	return 0;
}

void getSubmachineName(int indexMachine, Table symbolTable, char* name) {
	int i;

	for(i = 1; symbolTable.elem[indexMachine][i] != '>'; i++)
		name[i-1] = symbolTable.elem[indexMachine][i];
	name[i-1] = '\0';
}


void createDynamicTable(DynamicTable* table) {
	*table = NULL;
}

/**
 * Insert an new item on the table.
 *
 * @param table to be inserted into.
 * @param name to be inserted.
 *
 * @return the index of the added row, or the index of the existing row (if it already exists).
 */
int addToTable(DynamicTable* table, const char* name) {
	DynamicTable newCell, search;
	int index;

	index = lookUpForCell(*table, name);

	if(index == -1) {
		newCell = (DynamicTableNode*) malloc(sizeof(DynamicTableNode));

		newCell->name = (char*) malloc((strlen(name)+1)*sizeof(char));
		strcpy(newCell->name, name);

		newCell->next = NULL;

		if(*table == NULL) {
			*table = newCell;
			index = 0;
		}
		else {
			for(index = 1, search = *table; search->next != NULL; search = search->next, index++);

			search->next = newCell;
		}
	}

	return index;
}

/**
 * Looks for the existence of specified name entry in the Dynamic Table.
 *
 * @param table to look into.
 * @param name content in the row of the table.
 *
 * @return the index of the item in the table, -1 if not found.
 */
int lookUpForCell(DynamicTable table, const char* name) {
	DynamicTable search;
	int i;

	for(i = 0, search = table; search != NULL && strcmp(search->name, name); search = search->next, i++);

	if(search == NULL)
		return -1;
	else
		return i;
}
