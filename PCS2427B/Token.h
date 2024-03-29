/*
 * Token.h
 *
 *  Created on: 19/08/2012
 *      Author: Vinicius
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include "Table.h"

/*** TOKEN ***
 * NULL
 * IDENTIFIER
 * (KEYWORD)
 * CONSTANT
 * (OPERATOR)
 * PUNCTUATOR
 * STRING
 *************/

typedef struct {
	char type[17];
	char value[128];
} TokenStruct;

typedef TokenStruct* Token;

void createToken(Token* token);
int stringToInteger(const char* number, int base);
char* integerToString(char* string, int value, int base);
int characterToInteger(const char* character);
void copyToken(Token *dest, Token source);
void verifyKeyword(Table keywords, Token* token);

#endif /* TOKEN_H_ */
