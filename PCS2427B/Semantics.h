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
void startSemantics(FILE* file, DynamicTable* symbols);

/******** GENERAL ********/
void nil(FILE* file, DynamicTable* symbols, Token token);
void setIdentifier(FILE* file, DynamicTable* symbols, Token token);
void setConstant(FILE* file, DynamicTable* symbols, Token token);
void setType(FILE* file, DynamicTable* symbols, Token token);
void endBlock(FILE* file, DynamicTable* symbols, Token token);
void startFile(FILE* file, DynamicTable* symbols, Token token);
void endFile(FILE* file, DynamicTable* symbols, Token token);

/******** LIBRARY ********/

/******** VARIABLE ********/
void setVar(FILE* file, DynamicTable* symbols, Token token);

/******** PROCEDURE ********/
void setProcedure(FILE* file, DynamicTable* symbols, Token token);
void setLink(FILE* file, DynamicTable* symbols, Token token);
void clearParams(FILE* file, DynamicTable* symbols, Token token);
void setParam(FILE* file, DynamicTable* symbols, Token token);

/******** LABEL ********/
void setLabel(FILE* file, DynamicTable* symbols, Token token);

/******** JUMP ********/
void setJump(FILE* file, DynamicTable* symbols, Token token);

/******** CONDITIONAL ********/
void setConditional(FILE* file, DynamicTable* symbols, Token token);
void endConditional(FILE* file, DynamicTable* symbols, Token token);
void noElse(FILE* file, DynamicTable* symbols, Token token);
void endElse(FILE* file, DynamicTable* symbols, Token token);

/******** LOOP ********/
void setLoop(FILE* file, DynamicTable* symbols, Token token);
void setIteration(FILE* file, DynamicTable* symbols, Token token);
void endIteration(FILE* file, DynamicTable* symbols, Token token);

/******** SUBROTINE CALL ********/
void setReturn(FILE* file, DynamicTable* symbols, Token token);
void setReturn2(FILE* file, DynamicTable* symbols, Token token);

/******** ATTRIBUTION ********/
void setDest(FILE* file, DynamicTable* symbols, Token token);
void setOperator(FILE* file, DynamicTable* symbols, Token token);
void setAttribution(FILE* file, DynamicTable* symbols, Token token);


#endif /* SEMANTIC_H_ */
