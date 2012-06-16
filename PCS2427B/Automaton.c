/*
 * Automaton.c
 *
 *  Created on: 09/05/2012
 *      Author: Leonardo Pedro Kakitani
 *      		Marcos Vinicius Maciel da Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Table.h"
#include "Automaton.h"

void createGraph(FILE* input, Automaton* automaton) {
	char stateName[NAME_LENGTH], symbol[NAME_LENGTH], nextStateName[NAME_LENGTH];
	int stateIndex, symbolIndex, nextStateIndex;
	int i, j;

	*automaton = (AutomatonStruct*) malloc(sizeof(AutomatonStruct));

	createTable(input, &((*automaton)->stateTable));
	createTable(input, &((*automaton)->symbolTable));

	(*automaton)->production = (int**) malloc(((*automaton)->stateTable.size)*sizeof(int*));
	for(i = 0; i < (*automaton)->stateTable.size; i++)
		(*automaton)->production[i] = (int*) malloc(((*automaton)->symbolTable.size)*sizeof(int));

	if((*automaton)->production == NULL);

	for(i = 0; i < (*automaton)->stateTable.size; i++)
		for(j = 0; j < (*automaton)->symbolTable.size; j++)
			(*automaton)->production[i][j] = -1;


	while(!feof(input)) {
		fscanf(input, "%s %s %s", stateName, symbol, nextStateName);

		stateIndex = findIndex((*automaton)->stateTable, stateName);
		symbolIndex = findIndex((*automaton)->symbolTable, symbol);
		nextStateIndex = findIndex((*automaton)->stateTable, nextStateName);

		if(stateIndex < 0) {
			printf("ERROR: State \"%s\" undefined.\n", stateName);
			fflush(stdout);
			exit(2);
		}
		else if(symbolIndex < 0) {
			printf("ERROR: Symbol \"%s\" undefined.\n", symbol);
			fflush(stdout);
			exit(2);
		}
		else if(nextStateIndex < 0) {
			printf("ERROR: State \"%s\" undefined.\n", nextStateName);
			fflush(stdout);
			exit(2);
		}
		else
			(*automaton)->production[stateIndex][symbolIndex] = nextStateIndex;
	}
}
