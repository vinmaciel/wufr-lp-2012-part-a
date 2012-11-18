/*
 * Semantic.c
 *
 *  Created on: 16/11/2012
 *      Author: Vinicius
 */

#include <string.h>
#include <stdlib.h>
#include "Semantics.h"
#include "Table.h"


/******** PRIVATE PARAMETERS ********/
char identifier[64];
char label[64];
/************************************/

/**
 * Retrieves the index associated to the semantic action.
 *
 * @param label that describes the action.
 *
 * @return the index of the action.
 *
 */
int getSemanticFunctionIndex(const char* label) {
	if(!strcmp(label, "nil"))
		return 0;
	if(!strcmp(label, "setIdentifier"))
		return 1;
	if(!strcmp(label, "setLabel"))
		return 2;
	if(!strcmp(label, "endBlock"))
		return 3;
	if(!strcmp(label, "setJump"))
		return 4;
	if(!strcmp(label, ""))
		return 5;

	return -1;
}

/**
 * Call which semantic function must be called, represented by its string label.
 *
 * @param index associated to the semantic function.
 *
 * @return the semantic function.
 */
semantic semanticFunction(int index) {
	switch(index) {
	case 0:
		return nil;
	case 1:
		return setIdentifier;
	case 2:
		return setLabel;
	case 3:
		return endBlock;
	case 4:
		return setJump;
	}
	return nil;
}

void cleanSemanticParameters() {
	strcpy(identifier, "");
	strcpy(label, "");
}

/******** GENERAL ********/
// 0
void nil(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: nil\n");fflush(stdout);
}
// 1
void setIdentifier(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: id\n");fflush(stdout);
	strcpy(identifier, token->value);
}
// 3
void endBlock(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: EOB\n");fflush(stdout);
	// cannot end block with opened label
	if(strcmp(label, "")) {
		printf("ERROR: label at the end of compound statement.\n");
		fprintf(file, "ERROR: label at the end of compound statement.\n");
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}
}
/******** LIBRARY ********/

/******** VARIABLE ********/

/******** PROCEDURE ********/

/******** LABEL ********/
// 2
void setLabel(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: label\n");fflush(stdout);
	// if instruction already has a label, jump to the last assigned label
	if(strcmp(label,"")) {
		addToTable(symbols, identifier);
		fprintf(file, "%s\tJP\t%s\n", identifier, label);
		fflush(file);
	}
	// otherwise, add it to symbol table and activate it as current label
	else {
		if(lookUpForCell(*symbols, identifier) == -1) {
			strcpy(label, identifier);
			strcpy(identifier, "");
			addToTable(symbols, label);
		}
		else {
			printf("ERROR: duplicate label %s.\n", identifier);
			fprintf(file, "ERROR: duplicate label %s.\n", identifier);
			fflush(stdout);
			fflush(file);
			system("PAUSE");
			exit(5);
		}
	}
}

/******** JUMP ********/
// 4
void setJump(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: jump\n");fflush(stdout);
	if(lookUpForCell(*symbols, identifier) >= 0) {
		fprintf(file, "%s\tJP\t%s\n", label, identifier);
		fflush(file);
		strcpy(label, "");
	}
	else {
		printf("ERROR: label \"%s\" but not defined.\n", identifier);
		fprintf(file, "ERROR: label \"%s\" but not defined.\n", identifier);
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}
}
/******** CONDITIONAL ********/

/******** LOOP ********/

/******** SUBROTINE CALL ********/

/******** ATTRIBUTION ********/

