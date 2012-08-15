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
	getName(input, stateName);
	while(stateName[0] != EOF) {
		getName(input, symbol);
		getName(input, nextStateName);

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

		getName(input, stateName);
	}
}

void createAutomaton(FILE* input, Automaton* automaton) {
	char stateName[NAME_LENGTH], symbol[NAME_LENGTH], nextStateName[NAME_LENGTH];
	int stateIndex, symbolIndex, nextStateIndex;
	int i, j;

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
	getName(input, stateName);
	while(stateName[0] != EOF) {
		getName(input, symbol);
		getName(input, nextStateName);

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
		else // set a new transition
			(*automaton)->production[stateIndex][symbolIndex] = nextStateIndex;

		getName(input, stateName);
	}

	fclose(input);

	printf("Automaton created successfully.\n");
	fflush(stdout);
}

void generateTokenFile(Automaton lexer, const char* inputFileName) {
	int currentStateIndex, nextStateIndex, symbolIndex;
	char symbol[2];
	char recycled;
	int linefeed;
	char fileName[64];
	FILE *input, *output;

	input = fopen(inputFileName, "r");
	if(input == NULL) {
		printf("ERROR: input file not found.\n");
		fflush(stdout);
		system("PAUSE");
		exit(3);
	}
	strcpy(fileName, inputFileName);
	strcat(fileName, ".token");
	output = fopen(fileName, "w");
	if(output == NULL) {
		printf("ERROR: cannot create output file.\n");
		fflush(stdout);
		system("PAUSE");
		exit(5);
	}

	// initiate symbol
	symbol[0] = 1;
	symbol[1] = '\0';
	recycled = 0;
	linefeed = 0;

	for(currentStateIndex = 0; symbol[0] != EOF; currentStateIndex = nextStateIndex) {
		// get a symbol
		if(!recycled)
			symbol[0] = getc(input);
		else
			symbol[0] = recycled;

		linefeed = 0;

		if(isalpha(symbol[0]))
			symbol[0] = 'a';
		if(isdigit(symbol[0]))
			symbol[0] = 'n';
		if(isspace(symbol[0])) {
			if(symbol[0] == '\n')
				linefeed = 1;
			symbol[0] = ' ';
		}
		if(iscntrl(symbol[0]))
			symbol[0] = 0;
		if(symbol[0] == '\\' || symbol[0] == '\'' || symbol[0] == '@')
			symbol[0] = 's';

		recycled = 0;

		// if not EOF
		if(symbol[0] != EOF) {
			// get symbol index
			symbolIndex = findIndex(lexer->symbolTable, symbol);

			// if symbol exists in the alphabet, try the production
			if(symbolIndex >= 0)
				nextStateIndex = lexer->production[currentStateIndex][symbolIndex];
			// else, ERROR
			else {
				fprintf(output, "<ERROR: Symbol \"%c\" (%x) undefined>\n", symbol[0], symbol[0]); fflush(output);
				printf("ERROR: Symbol \"%c\" undefined for the lexical analyzer.\n", symbol[0]); fflush(stdout);
				system("PAUSE");
				exit(2);
			}

		}

		// if next state doesn't allow the creation of a new token, return token and restart
		if(nextStateIndex < 0) {
			fprintf(output, "%s ", lexer->stateTable.elem[currentStateIndex] + 1);
			if(linefeed) fprintf(output, "\n");
			fflush(output);
			recycled = symbol[0];
			nextStateIndex = 0;
		}
	}

	fclose(input);
	fclose(output);
}
