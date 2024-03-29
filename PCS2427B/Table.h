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

#define NAME_LENGTH 64

typedef struct {
	char** elem;
	int size;
} Table;

void createTable(FILE* input, Table* table);
int findIndex(Table table, char* name);
void getName(FILE* input, char* newName);
int isAcceptState(int indexState, Table stateTable);
int isSubMachine(int indexSymbol, Table automataTable);
void getSubmachineName(int indexMachine, Table symbolTable, char* name);

typedef struct _DynamicTableNode {
	char* name;
	char* class;
	char* nick;
	int defined;
	struct _DynamicTableNode* next;
} DynamicTableNode;

typedef DynamicTableNode* DynamicTable;

void createDynamicTable(DynamicTable* table);
int addToTable(DynamicTable* table, const char* name, const char* nick, const char* class);
void defineRow(DynamicTable* table, int index);
int lookUpForCell(DynamicTable table, const char* name, const char* class);
int lookForUndefined(DynamicTable table);
DynamicTable getRow(DynamicTable table, int index);

#endif /* TABLE_H_ */
