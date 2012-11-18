/*
 * Semantic.h
 *
 *  Created on: 16/11/2012
 *      Author: Vinicius
 */

#ifndef SEMANTIC_H_
#define SEMANTIC_H_

typedef void (*semantic)();

/******** PARAMETERS ********/
char semantic_label[256];

/******** GENERAL ********/
int getSemanticFunctionIndex(const char* label);
semantic semanticFunction(int index);
void cleanSemanticParameters();

/******** LIBRARY ********/

/******** VARIABLE ********/

/******** PROCEDURE ********/

/******** LABEL ********/

/******** JUMP ********/

/******** CONDITIONAL ********/

/******** LOOP ********/

/******** SUBROTINE CALL ********/

/******** ATTRIBUTION ********/


#endif /* SEMANTIC_H_ */
