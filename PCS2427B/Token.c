/*
 * Token.c
 *
 *  Created on: 19/08/2012
 *      Author: Vinicius
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Token.h"

void createToken(Token* token) {
	(*token) = (TokenStruct*) malloc(sizeof(TokenStruct));

	strcpy((*token)->type, "NULL");
	strcpy((*token)->value, "");
}

int characterToInteger(const char* character) {
	if(character[1] == '\\') {
		switch(character[2]) {
		case '0':
			return 0;
			break;

		case 'a':
			return 7;
			break;

		case 'b':
			return 8;
			break;

		case 't':
			return 9;
			break;

		case 'n':
			return 10;
			break;

		case 'v':
			return 11;
			break;

		case 'f':
			return 12;
			break;

		case 'r':
			return 13;
			break;

		case 'e':
			return 27;
			break;

		default:
			return -1;
		}
	}
	else {
		return character[1];
	}
}

int stringToInteger(const char* number, int base) {
	int integer = 0;
	int i;

	for(i = 0; i < strlen(number); i++) {
		if(isalpha(number[i])) {
			if(isupper(number[i]))
				integer = integer*base + (number[i] - 'A' + 10);
			else
				integer = integer*base + (number[i] - 'a' + 10);
		}
		else if(isdigit(number[i]))
			integer = integer*base + (number[i] - '0');
	}

	return integer;
}

void copyToken(Token *dest, Token source) {
	strcpy((*dest)->type, source->type);
	strcpy((*dest)->value, source->value);
}

char* integerToString(char* string, int value, int base) {
	char number[128];
	int i;

	if(!value) {
		strcpy(string, "0");
		return string;
	}

	for(i = 0; value; i++) {
		if(value%base < 10)
			number[i] = value%base + '0';
		else
			number[i] = value%base + 'A';

		value /= base;
	}
	number[i] = '\0';

	for(i = 0; i < strlen(number); i++)
		string[i] = number[strlen(number)-1-i];
	string[i] = '\0';

	return string;
}

void verifyKeyword(Table keywords, Token* token) {
	char tokenType[32];
	int i, index;

	// IDENTIFIER AND KEYOWRDS
	if(!strcmp((*token)->type, "IDENTIFIER")) {
		if(findIndex(keywords, (*token)->value) >= 0) {
			for(i = 0; (*token)->value[i] != '\0'; i++)
				tokenType[i] = toupper((*token)->value[i]);
			tokenType[i] = '\0';

			strcpy((*token)->type, tokenType);
		}
	}

	// CONSTANT
	else if(!strcmp((*token)->type, "CONSTANT")) {
		if((*token)->value[0] == '\'') { // characters
			index = characterToInteger((*token)->value);
			integerToString((*token)->value, index, 2);
		}
		else { // integers
			// TODO: the base value can be modified in future exercises
			index = stringToInteger((*token)->value, 10);
			integerToString((*token)->value, index, 2);
		}
	}
}
