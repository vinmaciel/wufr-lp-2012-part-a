/*
 * Semantic.h
 *
 *  Created on: 16/11/2012
 *      Author: Vinicius
 */

#ifndef SEMANTIC_H_
#define SEMANTIC_H_

#include "Token.h"

typedef void (*semantic)(FILE*, DynamicTable*, Token);

int getSemanticFunctionIndex(const char* label);
semantic semanticFunction(int index);
void cleanSemanticParameters();

/******** GENERAL ********/
void nil(FILE* file, DynamicTable* symbols, Token token);
void setIdentifier(FILE* file, DynamicTable* symbols, Token token);
void setType(FILE* file, DynamicTable* symbols, Token token);
void endBlock(FILE* file, DynamicTable* symbols, Token token);
void endFile(FILE* file, DynamicTable* symbols, Token token);

/******** LIBRARY ********/

/******** VARIABLE ********/
void setVar(FILE* file, DynamicTable* symbols, Token token);

/******** PROCEDURE ********/

/******** LABEL ********/
void setLabel(FILE* file, DynamicTable* symbols, Token token);

/******** JUMP ********/
void setJump(FILE* file, DynamicTable* symbols, Token token);

/******** CONDITIONAL ********/

/******** LOOP ********/

/******** SUBROTINE CALL ********/

/******** ATTRIBUTION ********/


#endif /* SEMANTIC_H_ */
