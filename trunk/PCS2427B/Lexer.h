/*
 * stringManager.h
 *
 *  Created on: 14/06/2012
 *      Author: Vinicius
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>

typedef struct {
	char* string;
	int index;
} StringManagerStruct;

typedef StringManagerStruct* StringManager;

typedef char Token;

void startStringManager(StringManager* manager);
void getStringFromConsole(StringManager* manager);
void getStringFromFile(StringManager* manager, FILE* file);
Token getToken(StringManager* manager);
void recycleToken(StringManager* manager, Token token);
char* printString(StringManager manager);
char* printToken(StringManager manager, int tokenSize);


/** TOKENS **/
#define NULL_TOKEN 0
#define NUMBER 'a'
#define IDENTIFIER 'n'
#define ADD_OPERATOR '+'
#define SUB_OPERATOR '-'
#define MULT_OPERATOR '*'
#define DIV_OPERATOR '/'
#define ASSIGNMENT_OPERATOR '='
#define END_STATEMENT ';'
#define PARENTHESES_L '('
#define PARENTHESES_R ')'

#endif /* LEXER_H_ */
