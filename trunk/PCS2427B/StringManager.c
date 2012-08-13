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
	char newString[1024];

	fgets(newString, 1023, file);

	(*manager)->string = (char*) malloc((strlen(newString)+1)*sizeof(char));
	(*manager)->index = 0;

	strcpy((*manager)->string, newString);
}

/*
 * returns 1 if end-of-string
 * returns 0 otherwise
 */
int getSymbol(StringManager* manager, char* symbol) {
	// only gets a char
	for(symbol[0] = (*manager)->string[(*manager)->index]; isspace(symbol[0]); (*manager)->index++)
		symbol[0] = (*manager)->string[(*manager)->index];
	symbol[1] = '\0';

	(*manager)->index++;

	if(iscntrl(symbol[0]))
		return 1;

	return 0;
}

void recycleSymbol(StringManager* manager, char* symbol) {
	(*manager)->index -= strlen(symbol);
}

char* printString(StringManager manager) {
	return &(manager->string[manager->index]);
}
