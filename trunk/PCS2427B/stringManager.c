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


void startStringManager(StringManager* manager) {
	*manager = (StringManagerStruct*) malloc(sizeof(StringManagerStruct));

	(*manager)->string = NULL;
	(*manager)->index = 0;
}

void getStringFromConsole(StringManager* manager) {
	char newString[1024];

	gets(newString);
	cleanString(newString);

	(*manager)->string = (char*) malloc((strlen(newString)+1)*sizeof(char));
	(*manager)->index = 0;

	strcpy((*manager)->string, newString);
}

void getStringFromFile(StringManager* manager, FILE* file) {
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
int getToken(StringManager* manager, char* token) {
	// only gets a char
	for(token[0] = (*manager)->string[(*manager)->index]; isspace(token[0]); (*manager)->index++)
		token[0] = (*manager)->string[(*manager)->index];
	token[1] = '\0';

	(*manager)->index++;

	if(iscntrl(token[0]))
		return 1;

	return 0;
}

void recycleToken(StringManager* manager, char* token) {
	(*manager)->index -= strlen(token);
}

char* printString(StringManager manager) {
	return &(manager->string[manager->index]);
}

char* printToken(StringManager manager, int tokenSize) {
	char* string;
	int i;

	string = (char*) malloc((tokenSize+1)*sizeof(char));

	for(i = 0; i < tokenSize; i++) {
		string[i] = manager->string[manager->index+i];
	}
	string[i] = '\0';

	return string;
}

