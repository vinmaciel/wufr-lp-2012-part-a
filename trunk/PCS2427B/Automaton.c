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

/*
 * Generate a token based on an input file in C language.
 *
 * @return 0 if a valid token is generated
 * @return -1 if symbol undefined at lexer
 * @return -2 if symbol (not comment) larger than 256
 * @return -3 if end state is not accept state (valid token)
 */
int generateToken(FILE* input, Automaton lexer, char* recycled, Token* token) {
	int currentStateIndex, nextStateIndex, symbolIndex;
	char currentString[128], symbol[2];
	char currentCharacter;
	int index, threshold;

	// initiate symbol
	symbol[0] = 1;
	symbol[1] = '\0';
	currentString[0] = '\0';

	for(currentStateIndex = 0; symbol[0] != EOF; currentStateIndex = nextStateIndex) {
		if(!currentStateIndex) {
			index = 0;
			threshold = 0;
		}

		// get a symbol
		if(!(*recycled))
			currentCharacter = getc(input);
		else
			currentCharacter = *recycled;

		if(isalpha(currentCharacter))
			symbol[0] = 'a';
		else if(isdigit(currentCharacter))
			symbol[0] = 'n';
		else if(isspace(currentCharacter))
			symbol[0] = ' ';
		else if(iscntrl(currentCharacter))
			symbol[0] = 0;
		else if(currentCharacter == '@' || currentCharacter == '$')
			symbol[0] = 's';
		else
			symbol[0] = currentCharacter;

		*recycled = 0;

		// if not EOF
		if(symbol[0] != EOF) {
			// get symbol index
			symbolIndex = findIndex(lexer->symbolTable, symbol);

			// if symbol exists in the alphabet, try the production
			if(symbolIndex >= 0)
				nextStateIndex = lexer->production[currentStateIndex][symbolIndex];
			// else, ERROR
			else {
				*recycled = symbol[0];
				return -1;
			}
		}
		else {
			*recycled = EOF;
			return 1;
		}

		// if next state doesn't allow the creation of a new token, return token and restart
		if(nextStateIndex < 0) {
			if(threshold) {
				strcpy((*token)->value, currentString);
				return -2;
			}
			else if(isAcceptState(currentStateIndex, lexer->stateTable)) {
				currentString[index] = '\0';

				strcpy((*token)->type, lexer->stateTable.elem[currentStateIndex] + 1);
				strcpy((*token)->value, currentString);

				*recycled = currentCharacter;
				return 0;
			}
			else {
				strcpy((*token)->value, lexer->stateTable.elem[currentStateIndex]);
				return -3;
			}
		}

		if(index < 127) {
			currentString[index] = currentCharacter;
			index++;
		}
		else
			threshold = 1;
	}

	return -4;
}

void consumeFile(Automaton lexer, Table keywords, const char* inputFileName) {
	int i;
	char recycled = 0;
	Token token;
	DynamicTable symbols, search;
	char fileName[64], tokenType[32];
	FILE *input, *output;

	input = fopen(inputFileName, "r");
	if(input == NULL) {
		printf("ERROR: lexical analyzer file not found.\n");
		fflush(stdout);
		system("PAUSE");
		exit(3);
	}
	strcpy(fileName, inputFileName);
	strcat(fileName, ".token");
	output = fopen(fileName, "w");
	if(output == NULL) {
		printf("ERROR: cannot create output token file.\n");
		fflush(stdout);
		system("PAUSE");
		exit(5);
	}

	createToken(&token);
	createDynamicTable(&symbols);

	while(recycled != EOF) {
		switch(generateToken(input, lexer, &recycled, &token)) {
		case 0:
			if(!strcmp(token->type, "IDENTIFIER")) {
				if(findIndex(keywords, token->value) >= 0) {
					strcpy(tokenType, "KEYWORD_");
					for(i = 0; token->value[i] != '\0'; i++)
						tokenType[i+8] = toupper(token->value[i]);
					tokenType[i+8] = '\0';

					strcpy(token->type, tokenType);
				}
				else {
					addToTable(&symbols, token->value);
				}
			}

			fprintf(output, "%17s %s\n", token->type, token->value);
			fflush(output);
			break;

		case 1:
			break;

		case -1:
			fprintf(output, "<ERROR: Symbol \"%c\" (%x) undefined>\n", recycled, recycled); fflush(output);
			printf("ERROR: Symbol \"%c\" undefined for the lexical analyzer.\n", recycled); fflush(stdout);
			system("PAUSE");
			exit(2);
			break;

		case -2:
			fprintf(output, "<ERROR: Symbol \"%s\" larger than 256>\n", token->value); fflush(output);
			printf("ERROR: Symbol \"%s\" larger than 256.\n", token->value); fflush(stdout);
			system("PAUSE");
			exit(2);
			break;

		case -3:
			fprintf(output, "<ERROR: Token \"%s\" undefined>\n", token->value); fflush(output);
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
	}

	fclose(input);
	fclose(output);

	// create a file to print the symbol table
	strcpy(fileName, inputFileName);
	strcat(fileName, ".symbol");
	output = fopen(fileName, "w");

	for(i = 0, search = symbols; search != NULL; search = search->next, i++){
		fprintf(output, "%4d: %s\n", i, search->name);fflush(output);
	}
}
