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
#include "Lexer.h"
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
Token getToken(StringManager* manager) {
	char character;

	for(character = (*manager)->string[(*manager)->index]; isspace(character); character = (*manager)->string[++(*manager)->index]);

	(*manager)->index++;

	if(iscntrl(character))
		return NULL_TOKEN;

	/* IDENTIFIERs only accept character */
	if(isalpha(character)) {
		while(isalpha((*manager)->string[(*manager)->index]) || isspace((*manager)->string[(*manager)->index]))
			(*manager)->index++;
		return IDENTIFIER;
	}
	/* NUMBERs only accept integers */
	else if(isdigit(character)) {
		while(isdigit((*manager)->string[(*manager)->index]) || isspace((*manager)->string[(*manager)->index]))
			(*manager)->index++;
		return NUMBER;
	}
	else if(character == '+')
		return ADD_OPERATOR;
	else if(character == '-')
		return SUB_OPERATOR;
	else if(character == '*')
		return MULT_OPERATOR;
	else if(character == '/')
		return DIV_OPERATOR;
	else if(character == '=')
		return ASSIGNMENT_OPERATOR;
	else if(character == ';')
		return END_STATEMENT;
	else if(character == '(')
		return PARENTHESES_L;
	else if(character == ')')
		return PARENTHESES_R;
	else
		return NULL_TOKEN;
}

void recycleToken(StringManager* manager, Token token) {
	if(token == IDENTIFIER)
		while(isalpha((*manager)->string[(*manager)->index-1]) || isspace((*manager)->string[(*manager)->index-1]))
			(*manager)->index--;
	else if(token == NUMBER)
		while(isdigit((*manager)->string[(*manager)->index-1]) || isspace((*manager)->string[(*manager)->index-1]))
			(*manager)->index--;
	else {
		while(isspace((*manager)->string[(*manager)->index-1]))
			(*manager)->index--;
		(*manager)->index--;
	}
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

