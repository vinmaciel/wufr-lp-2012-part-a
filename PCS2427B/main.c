/*
 * main.c
 *
 *  Created on: 09/05/2012
 *      Author: Leonardo Pedro Kakitani
 *      		Marcos Vinicius Maciel da Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Automaton.h"
#include "AutomataList.h"
#include "Machine.h"
#include "StringManager.h"

int main() {
	Automaton lexer;
	Machine parser;
	Table keywords;
	char fileName[64];
	FILE* input;
	int accept;

	int i,j,k;
	AutomataList list;

	// create lexical analyzer
	input = fopen("Lexer", "r");
	if(input == NULL) {
		printf("ERROR: file \"Lexer\" not found.\n");
		fflush(stdout);
		system("PAUSE");
		exit(3);
	}
	createAutomaton(input, &lexer);

	// create syntax analyzer
	input = fopen("Syntax", "r");
	if(input == NULL) {
		printf("ERROR: file \"Syntax\" not found.\n");
		fflush(stdout);
		system("PAUSE");
		exit(3);
	}
	createMachine(input, &parser);

	for(i = 0, list = parser->automataList; i < parser->automataTable.size && list != NULL; i++, list = list->next) {
		printf("$machine: %s\n", parser->automataTable.elem[i]);fflush(stdout);

		if(list == NULL) {
			printf("Ooops\n");
			fflush(stdout);
		}
		else {

			for(j = 0; j < list->elem->symbolTable.size; j++) {
				for(k = 0; k < list->elem->stateTable.size; k++) {
					printf("%2d ", list->elem->production[0][k][j]);fflush(stdout);
				}
				printf("\n");fflush(stdout);
			}

			printf("\n");fflush(stdout);
		}
	}
	if(list == NULL) {
		printf("it's just it\n");fflush(stdout);
	}


	// get keyword table
	input = fopen("Keywords", "r");
	createTable(input, &keywords);
	fclose(input);

	// get input file name
	printf("Enter input file name:\n");
	fflush(stdout);
	gets(fileName);

	accept = consumeFile(lexer, parser, keywords, fileName);
	if(accept)
		printf("File accepted!\n");
	else
		printf("File not accepted!\n");
	fflush(stdout);

	system("PAUSE");

	return 0;
}
