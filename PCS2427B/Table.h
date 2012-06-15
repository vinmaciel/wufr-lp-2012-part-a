/*
 * Table.h
 *
 *  Created on: 09/05/2012
 *      Author: Leonardo Pedro Kakitani
 *      		Marcos Vinicius Maciel da Silva
 */

#ifndef TABLE_H_
#define TABLE_H_

#define NAME_LENGTH 32

typedef struct {
	char** elem;
	int size;
} Table;

void createTable(FILE* input, Table* table);
int findIndex(Table table, char* name);
void getName(FILE* input, char* newName);
int acceptState(int indexState, Table stateTable);

#endif /* TABLE_H_ */
