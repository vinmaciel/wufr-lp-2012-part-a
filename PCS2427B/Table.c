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
		getName(input, name);

		if(findIndex(*table, name) >= 0) {
			printf("ERROR: Table element \"%s\" redefined.\n", name);
			fflush(stdout);
			exit(1);
		}

		table->elem[i] = (char*) malloc((strlen(name)+1)*sizeof(char));

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
		fscanf(input, "%c", &letter);

		if(letter == '"')
			if(quotes == 0) {
				quotes = 1;
				i--;
			}
			else {
				name[i] = '\0';
				i = NAME_LENGTH;
			}
		else
			if(quotes != 1)
				i--;
			else
				name[i] = letter;
	}

	strcpy(newName, name);
}

int acceptState(int indexState, Table stateTable) {
	if(stateTable.elem[indexState][0] == '*')
		return 1;

	return 0;
}