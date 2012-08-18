/*
 * stringManager.c
 *
 *  Created on: 14/06/2012
 *      Author: Vinicius
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "StringManager.h"


void startStringManager(StringManager* manager) {
	*manager = (StringManagerStruct*) malloc(sizeof(StringManagerStruct));

	(*manager)->string = NULL;
	(*manager)->index = 0;
}

void getStringFromFile(StringManager* manager, FILE* file) {
	int i;
	char newString[1024];

	fgets(newString, 1023, file);

	(*manager)->string = (char*) malloc((strlen(newString)+1)*sizeof(char));
	(*manager)->index = 0;

	for(i = 0; i < strlen(newString); i++)
		if(iscntrl(newString[i])) newString[i] = '\0';

	strcpy((*manager)->string, newString);
}

/*
 * returns 1 if end-of-string
 * returns 0 otherwise
 */
char getSymbol(StringManager* manager) {
	char symbol;

	// only gets a char
	for(symbol = (*manager)->string[(*manager)->index]; isspace(symbol); (*manager)->index++)
		symbol = (*manager)->string[(*manager)->index];

	(*manager)->index++;

	if(isalpha(symbol))
		return 'a';
	if(isdigit(symbol))
		return 'n';
	if(iscntrl(symbol))
		return 0;
	else
		return symbol;
}

char getChar(FILE* input) {
	char symbol;

	for(symbol = getc(input); isspace(symbol); symbol = getc(input))
		printf("%x ", symbol);

	printf("%x ", symbol);
	fflush(stdout);

	if(isalpha(symbol))
		return 'a';
	if(isdigit(symbol))
		return 'n';
	if(iscntrl(symbol))
		return 0;
	else
		return symbol;
}

void recycleSymbol(StringManager* manager) {
	(*manager)->index--;
}

char* printString(StringManager manager) {
	return &(manager->string[manager->index]);
}


void createToken(Token* token) {
	(*token) = (TokenStruct*) malloc(sizeof(TokenStruct));

	strcpy((*token)->type, "NULL");
	strcpy((*token)->value, "");
}
