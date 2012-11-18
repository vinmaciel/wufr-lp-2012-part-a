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
#include "Token.h"
#include "AutomataList.h"
#include "AutomataStack.h"
#include "Semantics.h"

void createMachine(FILE* input, Machine* machine) {
	int i;
	char automatonFileName[64];
	Automaton automaton;
	FILE *automatonFile;

	/* create free list */
	newFreeList();

	*machine = (MachineStruct*) malloc(sizeof(MachineStruct));

	/* create the automata table */
	createTable(input, &((*machine)->automataTable));

	/* read each file that describes the automata */
	for(i = 0; i < (*machine)->automataTable.size; i++) {
		automatonFile = NULL;
		automatonFileName[0] = '\0';

		automaton = NULL;

		strcpy(automatonFileName, (*machine)->automataTable.elem[i]);

		printf("filename: %s\n", automatonFileName);fflush(stdout);

		automatonFile = fopen(automatonFileName, "r");

		if(automatonFile == NULL) {
			printf("ERROR: Automaton file \"%s\" not found.\n", automatonFileName);
			fflush(stdout);
			exit(3);
		}
		else
			createGraph(automatonFile, &automaton, (*machine)->automataTable);

		fclose(automatonFile);

		addAutomatonToList(&((*machine)->automataList), automaton);

		printf("Automaton \"%s\" created successfully.\n", (*machine)->automataTable.elem[i]);
		fflush(stdout);
	}

	fclose(input);

	printf("Machine loaded successfully.\n\n");
	fflush(stdout);
}

/*
 * Verifies if string from stringManager is accepted by the automaton described at the machine.
 * @return 1 if it accepts, 0 otherwise.
 */
int testString(Machine machine, StringManager* stringManager) {
	int i, j;
	int currentStateIndex, symbolIndex, nextStateIndex, currentAutomatonIndex;
	int noTransition, result;
	char symbol[2];
	Automaton currentAutomaton;
	AutomataStack stack;
	FILE* log;
	char logName[259];


	/* log: get file name */
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

	/* log: write input string */
	if(log != NULL) fprintf(log, "\tInput string:\t%s\n\n", (*stringManager)->string);

	/* get an empty stack */
	newAutomataStack(&stack);

	/* get the initial automaton */
	currentAutomatonIndex = 0;
	currentAutomaton = getAutomatonByIndex(machine->automataList, currentAutomatonIndex);
	if(currentAutomaton == NULL) {
		printf("ERROR: Searching for non-existing automaton.\n");
		fflush(stdout);
		exit(4);
	}

	symbol[0] = 1;
	symbol[1] = '\0';
	noTransition = 0;

	for(nextStateIndex = 0; symbol[0] && !noTransition && nextStateIndex >= 0; i++) {
		currentStateIndex = nextStateIndex;

		/* log: store state of the machine */
		if(log != NULL){
			fprintf(log, "Machine: %s\n", machine->automataTable.elem[currentAutomatonIndex]);
			fprintf(log, "State: %s\n", currentAutomaton->stateTable.elem[currentStateIndex]);
			fprintf(log, "Remaining String: %s\n", printString(*stringManager));
			fprintf(log, "Stack size: %d\n", stackSize(stack));
			fprintf(log, "---\n\n");
			fflush(log);
		}

		/* get symbol */
		symbol[0] = getSymbol(stringManager);
		symbolIndex = findIndex(currentAutomaton->symbolTable, symbol);

		if(symbol[0]) {
			/* do the production */
			if(symbolIndex >= 0)
				nextStateIndex = currentAutomaton->production[0][currentStateIndex][symbolIndex];

			/*
			 * if the symbol doesn't belong to the machine OR
			 * the production redirects to a rejection state
			 * => verify if there is a submachine call in this state
			 */
			if(symbolIndex < 0 || nextStateIndex < 0) {
				/* enter the submachine */
				if(currentAutomaton->submachine[0][currentStateIndex] >= 0) {
					pushAutomaton(&stack, currentAutomatonIndex, currentAutomaton->submachine[1][currentStateIndex]);

					currentAutomatonIndex = currentAutomaton->submachine[0][currentStateIndex];
					currentAutomaton = getAutomatonByIndex(machine->automataList, currentAutomatonIndex);
					nextStateIndex = 0;
					if(log != NULL) fprintf(log, "<Push>\n");
				}
				/* verify if there is a stacked submachine */
				else {
					if(!isEmptyStack(stack) && isAcceptState(currentStateIndex, currentAutomaton->stateTable)) {
						popAutomaton(&stack, &currentAutomatonIndex, &nextStateIndex);
						currentAutomaton = getAutomatonByIndex(machine->automataList, currentAutomatonIndex);
						if(log != NULL) fprintf(log, "<Pop>\n");
					}
					else
						noTransition = 1;
				}

				recycleSymbol(stringManager);
			}
		}
	}

	/* try to pop all automata */
	while(!isEmptyStack(stack) && nextStateIndex > 0 && isAcceptState(nextStateIndex, currentAutomaton->stateTable)) {
		popAutomaton(&stack, &currentAutomatonIndex, &nextStateIndex);
		currentAutomaton = getAutomatonByIndex(machine->automataList, currentAutomatonIndex);
		if(log != NULL) {
			fprintf(log, "<End-of-string... Popping>\n");
			fprintf(log, "Machine: %s\n", machine->automataTable.elem[currentAutomatonIndex]);
			fprintf(log, "State: %s\n", currentAutomaton->stateTable.elem[nextStateIndex]);
			fprintf(log, "Stack size: %d\n", stackSize(stack));
			fprintf(log, "---\n\n");
		}
	}

	/* log: write ending state */
	if(log != NULL) {
		if(nextStateIndex < 0)
			fprintf(log, "End State: REJECTION STATE.\n");
		else
			fprintf(log, "End State: %s\n", currentAutomaton->stateTable.elem[nextStateIndex]);

		fprintf(log, "Stack size: %d\n", stackSize(stack));
	}

	/* accept (or do not) the string */
	if(!symbol[0] && nextStateIndex > 0 && isAcceptState(nextStateIndex, currentAutomaton->stateTable) && isEmptyStack(stack)) {
		if(log != NULL) fprintf(log, "ACCEPTED!");
		result = 1;
	}
	else {
		if(log != NULL) fprintf(log, "REJECTED!");
		result = 0;
	}

	/* log: close file */
	if(log != NULL) {
		fprintf(log, "\n\n\n");
		fclose(log);
	}

	cleanAutomataStack(&stack);

	return result;
}

/**
 * Consumes characters from a file in order to provide a set of tokens that will be used to decide if the
 * sentences from the file belongs to the language specified.
 *
 * @return 1 if accepts, 0 otherwise
 */
int consumeFile(Automaton lexer, Machine parser, Table keywords, const char* inputFileName) {
	int i;
	int currentStateIndex, symbolIndex, nextStateIndex, currentAutomatonIndex;
	int noTransition, result;

	char recycled;
	Automaton currentAutomaton;
	AutomataStack stack;
	Token token, recycledToken;
	DynamicTable symbols, search;
	char fileName[64];
	FILE *input, *tokenized, *compiled, *symbol;

	semantic semanticAction;

	/* open input file */
	input = fopen(inputFileName, "r");
	if(input == NULL) {
		printf("ERROR: input file not found.\n");
		fflush(stdout);
		system("PAUSE");
		exit(3);
	}

	/* create token file */
	strcpy(fileName, inputFileName);
	strcat(fileName, ".token");
	tokenized = fopen(fileName, "w");
	if(tokenized == NULL) {
		printf("ERROR: cannot create output token file.\n");
		fflush(stdout);
		system("PAUSE");
		exit(5);
	}

	/* create compiled file */
	strcpy(fileName, inputFileName);
	strcat(fileName, ".out");
	compiled = fopen(fileName, "w");
	if(compiled == NULL) {
		printf("ERROR: cannot create output file.\n");
		fflush(stdout);
		system("PAUSE");
		exit(5);
	}

	/* get an empty stack */
	newAutomataStack(&stack);

	/* create token and symbol table */
	createToken(&token);
	createToken(&recycledToken);
	createDynamicTable(&symbols);

	noTransition = 0;
	recycled = 0;
	strcpy(recycledToken->type, "NULL");

	semanticAction = semanticFunction(0);

	/* get the initial automaton */
	currentAutomatonIndex = 0;
	currentAutomaton = getAutomatonByIndex(parser->automataList, currentAutomatonIndex);
	if(currentAutomaton == NULL) {
		printf("ERROR: Searching for non-existing automaton.\n");
		fflush(stdout);
		exit(4);
	}

	for(nextStateIndex = 0; recycled != EOF && !noTransition && nextStateIndex >= 0; i++) {
		currentStateIndex = nextStateIndex;

		printf("machine: %d :: state: %d\n", currentAutomatonIndex, currentStateIndex);fflush(stdout);
		/* get token */
		if(!strcmp(recycledToken->type, "NULL")) { /* if not recycled */
			printf("new token\n");fflush(stdout);
			switch(generateToken(input, lexer, &recycled, &token)) {
			case 1:
				break;

			case -1:
				fprintf(tokenized, "<ERROR: Symbol \"%c\" (%x) undefined>\n", recycled, recycled); fflush(tokenized);
				printf("ERROR: Symbol \"%c\" undefined for the lexical analyzer.\n", recycled); fflush(stdout);
				system("PAUSE");
				exit(2);
				break;

			case -2:
				fprintf(tokenized, "<ERROR: Symbol \"%s\" larger than 256>\n", token->value); fflush(tokenized);
				printf("ERROR: Symbol \"%s\" larger than 256.\n", token->value); fflush(stdout);
				system("PAUSE");
				exit(2);
				break;

			case -3:
				fprintf(tokenized, "<ERROR: Token \"%s\" undefined>\n", token->value); fflush(tokenized);
				printf("ERROR: Token \"%s\" undefined for the lexical analyzer.\n", token->value); fflush(stdout);
				system("PAUSE");
				exit(2);
				break;

			case -4:
				printf("ERROR: unknown.\n"); fflush(stdout);
				system("PAUSE");
				exit(3);
				break;
			}

			verifyKeyword(keywords, &token);
		}
		else {
			printf("recycled token\n");fflush(stdout);
			strcpy(token->type, recycledToken->type);
			strcpy(token->value, recycledToken->value);
		}
		strcpy(recycledToken->type, "NULL");

		symbolIndex = findIndex(currentAutomaton->symbolTable, token->type);

		if(recycled != EOF) {

			printf("token: %s %s\n", token->type, token->value);fflush(stdout);

			if(recycled != EOF) {
				printf("not EOF\n");fflush(stdout);
				/* do the production */
				if(symbolIndex >= 0) {
					nextStateIndex = currentAutomaton->production[0][currentStateIndex][symbolIndex];
					semanticAction = semanticFunction(currentAutomaton->production[1][currentStateIndex][symbolIndex]);
					printf("produce: %2d %2d => %2d\n", currentStateIndex, symbolIndex, nextStateIndex);fflush(stdout);

					semanticAction(compiled, &symbols, token);
				}

				/*
				 * if the symbol doesn't belong to the machine OR
				 * the production redirects to a rejection state
				 * => verify if there is a submachine call in this state
				 */
				if(symbolIndex < 0 || nextStateIndex < 0) {
					/* enter the submachine */
					if(currentAutomaton->submachine[0][currentStateIndex] >= 0) {
						printf("enter submachine => ");fflush(stdout);
						pushAutomaton(&stack, currentAutomatonIndex, currentAutomaton->submachine[1][currentStateIndex]);

						currentAutomatonIndex = currentAutomaton->submachine[0][currentStateIndex];
						printf("%d\n", currentAutomatonIndex);fflush(stdout);
						currentAutomaton = getAutomatonByIndex(parser->automataList, currentAutomatonIndex);

						nextStateIndex = 0;
					}
					/* verify if there is a stacked submachine */
					else {
						if(!isEmptyStack(stack) && isAcceptState(currentStateIndex, currentAutomaton->stateTable)) {
							printf("leave submachine => ");fflush(stdout);
							popAutomaton(&stack, &currentAutomatonIndex, &nextStateIndex);
							printf("%d\n", currentAutomatonIndex);fflush(stdout);
							currentAutomaton = getAutomatonByIndex(parser->automataList, currentAutomatonIndex);
						}
						else {
							printf("no transition\n");fflush(stdout);
							noTransition = 1;
						}
					}

					printf("recycled?\n");fflush(stdout);
					copyToken(&recycledToken, token);
				}
				else {
					fprintf(tokenized, "%17s %s\n", token->type, token->value);
					fflush(tokenized);
				}
			}
		}
	}

	printf("pop\n");fflush(stdout);
	/* try to pop all automata */
	while(!isEmptyStack(stack) && nextStateIndex > 0 && isAcceptState(nextStateIndex, currentAutomaton->stateTable)) {
		printf("iterate\n");fflush(stdout);
		popAutomaton(&stack, &currentAutomatonIndex, &nextStateIndex);
		currentAutomaton = getAutomatonByIndex(parser->automataList, currentAutomatonIndex);
	}

	printf("end\n");fflush(stdout);
	/* accept (or do not) the string */
	if(recycled == EOF && nextStateIndex > 0 && isAcceptState(nextStateIndex, currentAutomaton->stateTable) && isEmptyStack(stack))
		result = 1;
	else
		result = 0;

	cleanAutomataStack(&stack);

	// create a file to print the symbol table
	strcpy(fileName, inputFileName);
	strcat(fileName, ".symbol");
	symbol = fopen(fileName, "w");

	for(i = 0, search = symbols; search != NULL; search = search->next, i++){
		fprintf(symbol, "%4d: %s\n", i, search->name);fflush(symbol);
	}

	return result;
}

