/*
 * Semantic.c
 *
 *  Created on: 16/11/2012
 *      Author: Vinicius
 */

#include <string.h>
#include "Semantics.h"

/******** GENERAL ********/

/**
 * Call which semantic function must be called, represented by its string label.
 *
 * @param label associated to the semantic function.
 * @return the semantic function.
 */
int getSemanticFunctionIndex(const char* label) {
	if(!strcmp(label, "nil"))
		return 0;
	if(!strcmp(label, ""))
		return 1;
	if(!strcmp(label, ""))
		return 2;
	if(!strcmp(label, ""))
		return 3;
	if(!strcmp(label, ""))
		return 4;
	if(!strcmp(label, ""))
		return 5;

	return -1;
}

semantic semanticFunction(int index) {
	return NULL;
}

void cleanSemanticParameters() {
	strcpy(semantic_label, "");
}

/******** LIBRARY ********/

/******** VARIABLE ********/

/******** PROCEDURE ********/

/******** LABEL ********/

/******** JUMP ********/

/******** CONDITIONAL ********/

/******** LOOP ********/

/******** SUBROTINE CALL ********/

/******** ATTRIBUTION ********/

