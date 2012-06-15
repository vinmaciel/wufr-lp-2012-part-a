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
#include "stringManager.h"
#include "utils.h"


void startStringManager(StringManager** manager) {
	*manager = (StringManager*) malloc(sizeof(StringManager));

	(*manager)->string = NULL;
	(*manager)->index = 0;
}

void getStringFromConsole(StringManager** manager) {
	char newString[1024];

	gets(newString);
	cleanString(newString);

	(*manager)->string = (char*) malloc((strlen(newString)+1)*sizeof(char));
	(*manager)->index = 0;

	strcpy((*manager)->string, newString);
}

void getStringFromFile(StringManager** manager, FILE* file) {
	char newString[1024];

	fgets(newString, 1023, file);
	cleanString(newString);

	(*manager)->string = (char*) malloc((strlen(newString)+1)*sizeof(char));
	(*manager)->index = 0;

	strcpy((*manager)->string, newString);
}

/*
 * returns 1 if end-of-string
 * returns 0 otherwise
 */
int getSymbol(StringManager** manager, char* symbol) {
	// only gets a char
	symbol[0] = (*manager)->string[(*manager)->index];
	symbol[1] = '\0';

	(*manager)->index++;

	if(iscntrl(symbol[0]))
		return 1;

	return 0;
}

void recycleSymbol(StringManager** manager, char* symbol) {
	(*manager)->index -= strlen(symbol);
}

char* printString(StringManager* manager) {
	return &(manager->string[manager->index]);
}

char* printSymbol(StringManager* manager, int symbolSize) {
	char* string;
	int i;

	string = (char*) malloc((symbolSize+1)*sizeof(char));

	for(i = 0; i < symbolSize; i++) {
		string[i] = manager->string[manager->index+i];
	}
	string[i] = '\0';

	return string;
}

