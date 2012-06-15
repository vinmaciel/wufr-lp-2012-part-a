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
#include "StringManager.h"

void createGraph(FILE* input, Automaton** automaton) {
	char stateName[NAME_LENGTH], symbol[NAME_LENGTH], nextStateName[NAME_LENGTH];
	int stateIndex, symbolIndex, nextStateIndex;
	int i, j;

	*automaton = (Automaton*) malloc(sizeof(Automaton));

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

int testString(Automaton automaton, StringManager* stringManager) {
	int i, j;
	int currentStateIndex, symbolIndex;
	int endOfString;
	char* symbol;
	FILE* log;
	char logName[259];

	// get log file name
	strcpy(logName, "log\\");
	for(i = 0, j = 0; stringManager->string[i] != '\0'; i++) {
		if(isalnum(stringManager->string[i])) {
			logName[4+j] = stringManager->string[i];
			j++;
		}
	}
	logName[4+i] = '\0';
	strcat(logName, ".txt");
	log = fopen(logName, "a");

	// log: write input string
	fprintf(log, "\tInput string:\t%s\n\n", stringManager->string);

	// get first symbol
	symbol = (char*) malloc(2*sizeof(char));
	endOfString = getSymbol(&stringManager, symbol);
	for(endOfString = 0, currentStateIndex = 0; !endOfString && currentStateIndex >= 0; i++) {
		symbolIndex = findIndex(automaton.symbolTable, symbol);

		if(log != NULL)
			fprintf(log, "State: %s\nRemaining String: %s\n---\n", automaton.stateTable.elem[currentStateIndex], printString(stringManager));

		// do the transition
		if(symbolIndex >= 0)
			currentStateIndex = automaton.production[currentStateIndex][symbolIndex];

		// get a new symbol
			endOfString = getSymbol(&stringManager, symbol);
	}
	if(log != NULL) {
		if(currentStateIndex < 0)
			fprintf(log, "End State: REJECTION STATE.\n");
		else
			fprintf(log, "End State: %s\n", automaton.stateTable.elem[currentStateIndex]);
	}

	// close log
	fprintf(log, "\n\n\n");
	fclose(log);

	// accept (or don't) the string
	if(currentStateIndex > 0 && acceptState(currentStateIndex, automaton.stateTable)) return 1;
	else return 0;
}
