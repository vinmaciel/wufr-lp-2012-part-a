/*
 * Table.h
 *
 *  Created on: 09/05/2012
 *      Author: Leonardo Pedro Kakitani
 *      		Marcos Vinicius Maciel da Silva
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <stdio.h>

#define NAME_LENGTH 32

typedef struct {
	char** elem;
	int size;
} Table;

void createTable(FILE* input, Table* table);
int findIndex(Table table, char* name);
int findTokenIndex(Table table, char token);
void getName(FILE* input, char* newName);
int isAcceptState(int indexState, Table stateTable);
int isSubMachine(int indexToken, Table automataTable);
void getSubmachineName(int indexMachine, Table tokenTable, char* name);

#endif /* TABLE_H_ */