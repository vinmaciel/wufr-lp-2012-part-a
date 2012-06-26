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
	char stateName[NAME_LENGTH], token[NAME_LENGTH], nextStateName[NAME_LENGTH];
	char submachineName[64];
	int stateIndex, tokenIndex, nextStateIndex;
	int i, j;

	submachineName[0] = '\0';

	// allocate memory to automaton (struct)
	*automaton = (AutomatonStruct*) malloc(sizeof(AutomatonStruct));

	// store token and state tables
	createTable(input, &((*automaton)->stateTable));
	createTable(input, &((*automaton)->tokenTable));

	// allocate memory to the production table
	(*automaton)->production = (int**) malloc(((*automaton)->stateTable.size)*sizeof(int*));
	for(i = 0; i < (*automaton)->stateTable.size; i++)
		(*automaton)->production[i] = (int*) malloc(((*automaton)->tokenTable.size)*sizeof(int));
	// allocate memory to the submachine-call table
	(*automaton)->submachine[0] = (int*) malloc(((*automaton)->stateTable.size)*sizeof(int));
	(*automaton)->submachine[1] = (int*) malloc(((*automaton)->stateTable.size)*sizeof(int));

	// set all productions to rejection state
	for(i = 0; i < (*automaton)->stateTable.size; i++)
		for(j = 0; j < (*automaton)->tokenTable.size; j++)
			(*automaton)->production[i][j] = -1;
	// clear all submachine calls
	for(i = 0; i < (*automaton)->tokenTable.size; i++) {
		(*automaton)->submachine[0][i] = -1;
		(*automaton)->submachine[1][i] = -1;
	}


	// read the transitions (productions and submachine calls)
	getName(input, stateName);
	while(stateName[0] != EOF) {
		getName(input, token);
		getName(input, nextStateName);

		// get the indexes (current state, token consumed, next state)
		stateIndex = findIndex((*automaton)->stateTable, stateName);
		tokenIndex = findIndex((*automaton)->tokenTable, token);
		nextStateIndex = findIndex((*automaton)->stateTable, nextStateName);

		// verify validity
		if(stateIndex < 0) {
			printf("ERROR: State \"%s\" undefined.\n", stateName);
			fflush(stdout);
			exit(2);
		}
		else if(tokenIndex < 0) {
			printf("ERROR: Token \"%s\" undefined.\n", token);
			fflush(stdout);
			exit(2);
		}
		else if(nextStateIndex < 0) {
			printf("ERROR: State \"%s\" undefined.\n", nextStateName);
			fflush(stdout);
			exit(2);
		}
		else { // set a new transition
			if(isSubMachine(tokenIndex, (*automaton)->tokenTable)) {
				getSubmachineName(tokenIndex, (*automaton)->tokenTable, submachineName);
				(*automaton)->submachine[0][stateIndex] = findIndex(automataTable, submachineName);
				(*automaton)->submachine[1][stateIndex] = nextStateIndex;
			}
			else
				(*automaton)->production[stateIndex][tokenIndex] = nextStateIndex;
		}

		getName(input, stateName);
	}
}
