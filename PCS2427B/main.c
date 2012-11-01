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
	Table keywords;
	char fileName[64];
	FILE* input;

	// create the automaton
	input = fopen("WirthLexer", "r");
	createAutomaton(input, &lexer);

	// get keyword table
	input = fopen("Keywords", "r");
	createTable(input, &keywords);
	fclose(input);

	// get input file name
	printf("Enter input file name:\n");
	fflush(stdout);
	gets(fileName);
	consumeFile(lexer, keywords, fileName);

	system("PAUSE");

	return 0;
}
