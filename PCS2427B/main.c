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
	Machine machine;
	FILE* input;
	StringManager stringManager;
	char fileName[64];

	// initiate string manager
	startStringManager(&stringManager);

	// get automaton file
	input = NULL;
	while (input == NULL) {
		printf("Enter automaton file name:\n");
		fflush(stdout);
		gets(fileName);
		input = fopen(fileName, "r");
	}

	// create the automaton
	createMachine(input, &machine);

	// close automaton file
	fclose(input);
	printf("Machine loaded successfully.\n\n");
	fflush(stdout);

	// choose input mode
	printf("Enter input file name:\n");
	fflush(stdout);
	gets(fileName);

	// get a new string from file
	input = fopen(fileName, "r");
	while(input == NULL) {
		printf("ERROR: input file not found. Choose a new one:\n");
		fflush(stdout);
		gets(fileName);
		input = fopen(fileName, "r");
	}

	getStringFromFile(&stringManager, input);

	while(1) {
		if(testString(machine, &stringManager))
			printf("\nString \"%s\" accepted by the automaton.\n", stringManager->string);
		else
			printf("\nString \"%s\" not accepted by the automaton.\n", stringManager->string);
		fflush(stdout);

		getStringFromFile(&stringManager, input);

		if(feof(input))
			break;
	}

	fclose(input);

	system("PAUSE");

	return 0;
}
