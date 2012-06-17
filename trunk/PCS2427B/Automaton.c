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

void createGraph(FILE* input, Automaton* automaton, Table automataTable) {
	char stateName[NAME_LENGTH], symbol[NAME_LENGTH], nextStateName[NAME_LENGTH];
	char submachineName[64];
	int stateIndex, symbolIndex, nextStateIndex;
	int i, j;

	submachineName[0] = '\0';

	// allocate memory to automaton (struct)
	*automaton = (AutomatonStruct*) malloc(sizeof(AutomatonStruct));

	// store symbol and state tables
	createTable(input, &((*automaton)->stateTable));
	createTable(input, &((*automaton)->symbolTable));

	// allocate memory to the production table
	(*automaton)->production = (int**) malloc(((*automaton)->stateTable.size)*sizeof(int*));
	for(i = 0; i < (*automaton)->stateTable.size; i++)
		(*automaton)->production[i] = (int*) malloc(((*automaton)->symbolTable.size)*sizeof(int));
	// allocate memory to the submachine-call table
	(*automaton)->submachine[0] = (int*) malloc(((*automaton)->stateTable.size)*sizeof(int));
	(*automaton)->submachine[1] = (int*) malloc(((*automaton)->stateTable.size)*sizeof(int));

	// set all productions to rejection state
	for(i = 0; i < (*automaton)->stateTable.size; i++)
		for(j = 0; j < (*automaton)->symbolTable.size; j++)
			(*automaton)->production[i][j] = -1;
	// clear all submachine calls
	for(i = 0; i < (*automaton)->symbolTable.size; i++) {
		(*automaton)->submachine[0][i] = -1;
		(*automaton)->submachine[1][i] = -1;
	}


	// read the transitions (productions and submachine calls)
	while(!feof(input)) {
		fscanf(input, "%s %s %s", stateName, symbol, nextStateName);

		// get the indexes (current state, symbol consumed, next state)
		stateIndex = findIndex((*automaton)->stateTable, stateName);
		symbolIndex = findIndex((*automaton)->symbolTable, symbol);
		nextStateIndex = findIndex((*automaton)->stateTable, nextStateName);

		// verify validity
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
		else { // set a new transition
			if(isSubMachine(symbolIndex, (*automaton)->symbolTable)) {
				getSubmachineName(symbolIndex, (*automaton)->symbolTable, submachineName);
				(*automaton)->submachine[0][stateIndex] = findIndex(automataTable, submachineName);
				(*automaton)->submachine[1][stateIndex] = nextStateIndex;
			}
			else
				(*automaton)->production[stateIndex][symbolIndex] = nextStateIndex;
		}
	}
}
