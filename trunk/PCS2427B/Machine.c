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
#include "AutomataStack.h"

void createMachine(FILE* input, Machine* machine) {
	int i;
	char automatonFileName[64];
	Automaton automaton;
	FILE* automatonFile;

	// create free list
	newFreeList();

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
			createGraph(automatonFile, &automaton, (*machine)->automataTable);

		printf("Automaton \"%s\" created successfully.\n", (*machine)->automataTable.elem[i]);
		fflush(stdout);
		fclose(automatonFile);

		addAutomatonToList(&((*machine)->automataList), automaton);
	}
}

int testString(Machine machine, StringManager* stringManager) {
	int i, j;
	int currentStateIndex, symbolIndex, nextStateIndex, currentAutomatonIndex;
	int endOfString, noTransition, result;
	char* symbol;
	Automaton currentAutomaton;
	AutomataStack stack;
	FILE* log;
	char logName[259];


	// log: get file name
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

	// get an empty stack
	newAutomataStack(&stack);

	// get the initial automaton
	currentAutomatonIndex = 0;
	currentAutomaton = getAutomatonByIndex(machine->automataList, currentAutomatonIndex);
	if(currentAutomaton == NULL) {
		printf("ERROR: Searching for non-existing automaton.\n");
		fflush(stdout);
		exit(4);
	}

	symbol = (char*) malloc(2*sizeof(char)); // only char
	endOfString = 0;
	noTransition = 0;

	for(nextStateIndex = 0; !endOfString && !noTransition && nextStateIndex >= 0; i++) {
		currentStateIndex = nextStateIndex;

		// log: store state of the machine
		if(log != NULL){
			fprintf(log, "Machine: %s\n", machine->automataTable.elem[currentAutomatonIndex]);
			fprintf(log, "State: %s\n", currentAutomaton->stateTable.elem[currentStateIndex]);
			fprintf(log, "Remaining String: %s\n", printString(*stringManager));
			fprintf(log, "Stack size: %d\n", stackSize(stack));
			fprintf(log, "---\n\n");
			fflush(log);
		}

		// get symbol
		endOfString = getSymbol(stringManager, symbol);
		symbolIndex = findIndex(currentAutomaton->symbolTable, symbol);

		if(!endOfString) {
			// do the production
			if(symbolIndex >= 0)
				nextStateIndex = currentAutomaton->production[currentStateIndex][symbolIndex];

			/*
			 * if the symbol doesn't belong to the machine OR
			 * the production redirects to a rejection state
			 * => verify if there is a submachine call in this state
			 */
			if(symbolIndex < 0 || nextStateIndex < 0) {
				// enter the submachine
				if(currentAutomaton->submachine[0][currentStateIndex] >= 0) {
					pushAutomaton(&stack, currentAutomatonIndex, currentAutomaton->submachine[1][currentStateIndex]);

					currentAutomatonIndex = currentAutomaton->submachine[0][currentStateIndex];
					currentAutomaton = getAutomatonByIndex(machine->automataList, currentAutomatonIndex);
					nextStateIndex = 0;
					fprintf(log, "<Push>\n");
				}
				// verify if there is a stacked submachine
				else {
					if(!isEmptyStack(stack) && isAcceptState(currentStateIndex, currentAutomaton->stateTable)) {
						popAutomaton(&stack, &currentAutomatonIndex, &nextStateIndex);
						currentAutomaton = getAutomatonByIndex(machine->automataList, currentAutomatonIndex);
						fprintf(log, "<Pop>\n");
					}
					else
						noTransition = 1;
				}

				recycleSymbol(stringManager, symbol);
			}
		}
	}

	// try to pop all automata
	while(!isEmptyStack(stack) && nextStateIndex > 0 && isAcceptState(nextStateIndex, currentAutomaton->stateTable)) {
		popAutomaton(&stack, &currentAutomatonIndex, &nextStateIndex);
		currentAutomaton = getAutomatonByIndex(machine->automataList, currentAutomatonIndex);
		fprintf(log, "<End-of-string... Popping>\n");
	}

	// log: write ending state
	if(log != NULL) {
		if(nextStateIndex < 0)
			fprintf(log, "End State: REJECTION STATE.\n");
		else
			fprintf(log, "End State: %s\n", currentAutomaton->stateTable.elem[nextStateIndex]);

		fprintf(log, "Stack size: %d\n", stackSize(stack));
	}

	// accept (or don't) the string
	if(nextStateIndex > 0 && isAcceptState(nextStateIndex, currentAutomaton->stateTable) && isEmptyStack(stack)) {
		fprintf(log, "ACCEPTED!");
		result = 1;
	}
	else {
		fprintf(log, "REJECTED!");
		result = 0;
	}

	// log: close file
	fprintf(log, "\n\n\n");
	fclose(log);

	cleanAutomataStack(&stack);

	return result;
}
