/*
 * machine.c
 *
 *  Created on: 15/06/2012
 *      Author: Vinicius
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Table.h"
#include "Machine.h"
#include "AutomataList.h"

void createMachine(FILE* input, Machine* machine) {
	int i;
	char automatonFileName[64];
	Automaton automaton;
	FILE* automatonFile;

	*machine = (MachineStruct*) malloc(sizeof(MachineStruct));

	// create the automata table
	createTable(input, &((*machine)->automataTable));

	// read each file that describes the automata
	for(i = 0; i < (*machine)->automataTable.size; i++) {
		automatonFile = NULL;
		automatonFileName[0] = '\0';

		strcpy(automatonFileName, (*machine)->automataTable.elem[i]);
		strcat(automatonFileName, ".txt");

		automatonFile = fopen(automatonFileName, "r");

		if(automatonFile == NULL) {
			printf("ERROR: Automaton file \"%s\" not found.\n", automatonFileName);
			fflush(stdout);
			exit(3);
		}
		else
			createGraph(automatonFile, &automaton);

		printf("Automaton \"%s\" created successfully.\n", (*machine)->automataTable.elem[i]);
		fflush(stdout);
		fclose(automatonFile);

		addAutomatonToList(&((*machine)->automataList), automaton);
	}
}

int testString(Machine machine, StringManager* stringManager) {
	int i, j;
	int currentStateIndex, symbolIndex;
	int endOfString;
	char* symbol;
	Automaton currentAutomaton;
	FILE* log;
	char logName[259];

	// get log file name
	strcpy(logName, "log\\");
	for(i = 0, j = 0; !iscntrl((*stringManager)->string[i]); i++) {
		if(isalnum((*stringManager)->string[i])) {
			logName[4+j] = (*stringManager)->string[i];
			j++;
		}
	}
	logName[4+j] = '\0';
	strcat(logName, ".txt");
	log = fopen(logName, "a");

	// log: write input string
	fprintf(log, "\tInput string:\t%s\n\n", (*stringManager)->string);

	// get the first automaton
	currentAutomaton = getAutomatonByIndex(machine->automataList, 0);
	if(currentAutomaton == NULL) {
		printf("ERROR: Searching for non-existing automaton.\n");
		fflush(stdout);
		exit(4);
	}

	// get first symbol
	symbol = (char*) malloc(2*sizeof(char));
	endOfString = getSymbol(stringManager, symbol);
	for(currentStateIndex = 0; !endOfString && currentStateIndex >= 0; i++) {
		symbolIndex = findIndex(currentAutomaton->symbolTable, symbol);

		if(log != NULL)
			fprintf(log, "State: %s\nRemaining String: %s\n---\n", currentAutomaton->stateTable.elem[currentStateIndex], printString(*stringManager));

		// do the transition
		if(symbolIndex >= 0)
			currentStateIndex = currentAutomaton->production[currentStateIndex][symbolIndex];

		// get a new symbol
		endOfString = getSymbol(stringManager, symbol);
	}
	if(log != NULL) {
		if(currentStateIndex < 0)
			fprintf(log, "End State: REJECTION STATE.\n");
		else
			fprintf(log, "End State: %s\n", currentAutomaton->stateTable.elem[currentStateIndex]);
	}

	// close log
	fprintf(log, "\n\n\n");
	fclose(log);

	// accept (or don't) the string
	if(currentStateIndex > 0 && acceptState(currentStateIndex, currentAutomaton->stateTable)) return 1;
	else return 0;
}
