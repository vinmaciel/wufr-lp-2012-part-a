/*
 * Semantic.c
 *
 *  Created on: 16/11/2012
 *      Author: Vinicius
 */

#include <string.h>
#include <stdlib.h>
#include "Semantics.h"
#include "Token.h"
#include "Table.h"


/******** PRIVATE PARAMETERS ********/
char identifier[64];
int constant;
char label[64];
char type[5];
char destination[64];
char operator[2];
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
	if(!strcmp(label, "endFile"))
		return 5;
	if(!strcmp(label, "setType"))
		return 6;
	if(!strcmp(label, "setVar"))
		return 7;
	if(!strcmp(label, "setConstant"))
		return 8;
	if(!strcmp(label, "setDest"))
		return 9;
	if(!strcmp(label, "setOperator"))
		return 10;
	if(!strcmp(label, "setAttribution"))
		return 11;

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
	case 5:
		return endFile;
	case 6:
		return setType;
	case 7:
		return setVar;
	case 8:
		return setConstant;
	case 9:
		return setDest;
	case 10:
		return setOperator;
	case 11:
		return setAttribution;
	}
	return nil;
}

void cleanSemanticParameters() {
	strcpy(identifier, "");
	strcpy(label, "");
	strcpy(type, "");
	strcpy(destination, "");
	strcpy(operator, "");

	constant = -1;
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
// 8
void setConstant(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: constant\n");fflush(stdout);
	constant = stringToInteger(token->value, 2);
}
// 6
void setType(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: type\n");fflush(stdout);
	strcpy(type, token->value);
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
// 5
void endFile(FILE* file, DynamicTable* symbols, Token token) {
	DynamicTable search;
	int i;

	i = lookForUndefined(*symbols);

	// cannot accept file with undefined labels
	if(i >= 0) {
		printf("ERROR: label \"%s\" used but not defined.\n", getRow(*symbols, i)->name);
		fprintf(file, "ERROR: label \"%s\" used but not defined.\n", getRow(*symbols, i)->name);
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}

	// the end of the file must contain the data allocation
	for(search = *symbols; search != NULL; search = search->next) {
		if(!strcmp(search->class, "variable")) {
			fprintf(file, "%s\t K\t/0000\n", search->name);
			fflush(file);
		}
	}

	// EOF
	fprintf(file, "\t#\n");
	fflush(file);
}
/******** LIBRARY ********/

/******** VARIABLE ********/
// 7
void setVar(FILE* file, DynamicTable* symbols, Token token) {
	int i;

	printf("semantic: var\n");fflush(stdout);
	i = lookUpForCell(*symbols, identifier, "variable");

	if(i == -1) {
		i = addToTable(symbols, identifier, "variable");
		defineRow(symbols, i);
		strcpy(identifier, "");
		strcpy(type, "");
	}
	else {
		printf("ERROR: redeclaration of \"%s\".\n", identifier);
		fprintf(file, "ERROR: redeclaration of \"%s\".\n", identifier);
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}
}
/******** PROCEDURE ********/

/******** LABEL ********/
// 2
void setLabel(FILE* file, DynamicTable* symbols, Token token) {
	int i;

	printf("semantic: label\n");fflush(stdout);
	// if instruction already has a label, jump to the last assigned label
	if(strcmp(label,"")) {
		i = addToTable(symbols, identifier, "label");
		defineRow(symbols, i);
		fprintf(file, "%s\tJP\t%s\n", identifier, label);
		fflush(file);
		strcpy(identifier, "");
	}
	// otherwise, add it to symbol table and activate it as current label
	else {
		i = lookUpForCell(*symbols, identifier, "label");
		if(i == -1) {
			strcpy(label, identifier);
			strcpy(identifier, "");
			i = addToTable(symbols, label, "label");
			defineRow(symbols, i);
		}
		else {
			if(!getRow(*symbols, i)->defined) {
				strcpy(label, identifier);
				strcpy(identifier, "");
				defineRow(symbols, i);
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
}

/******** JUMP ********/
// 4
void setJump(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: jump\n");fflush(stdout);
	if(lookUpForCell(*symbols, identifier, "label") == -1) {
		addToTable(symbols, identifier, "label");
	}

	fprintf(file, "%s\tJP\t%s\n", label, identifier);
	fflush(file);

	strcpy(label, "");
	strcpy(identifier, "");
}
/******** CONDITIONAL ********/

/******** LOOP ********/

/******** SUBROTINE CALL ********/

/******** ATTRIBUTION ********/
// 9
void setDest(FILE* file, DynamicTable* symbols, Token token) {
	int i;

	printf("semantic: dest\n");fflush(stdout);
	i = lookUpForCell(*symbols, identifier, "variable");

	if(i >= 0) {
		strcpy(destination, identifier);
		strcpy(identifier, "");
	}
	else {
		printf("ERROR: symbol \"%s\" could not be resolved.\n", identifier);
		fprintf(file, "ERROR: symbol \"%s\" could not be resolved.\n", identifier);
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}
}
// 10
void setOperator(FILE* file, DynamicTable* symbols, Token token) {
	int i;

	printf("semantic: operator\n");fflush(stdout);
	strcpy(operator, token->value);

	if(!strcmp(identifier, "")) {
		fprintf(file, "%s\tLV\t/%04X\n", label, constant);
		constant = -1;
	}
	else {
		i = lookUpForCell(*symbols, identifier, "variable");

		if(i >= 0) {
			fprintf(file, "%s\tLD\t%s\n", label, identifier);
			strcpy(identifier, "");
		}
	}
	fflush(file);
	strcpy(label, "");
}
// 11
void setAttribution(FILE* file, DynamicTable* symbols, Token token) {
	int i;

	printf("semantic: attribution\n");fflush(stdout);

	if(!strcmp(operator, "")) {
		if(!strcmp(identifier, "")) {
			fprintf(file, "%s\tLV\t/%04X\n", label, constant);
			fprintf(file, "\tMM\t%s\n", destination);
			fflush(file);
		}
		else {
			i = lookUpForCell(*symbols, identifier, "variable");

			if(i >= 0) {
				fprintf(file, "%s\tLD\t%s\n", label, identifier);
				fprintf(file, "\tMM\t%s\n", destination);
				fflush(file);
			}
			else {
				printf("ERROR: symbol \"%s\" could not be resolved.\n", identifier);
				fprintf(file, "ERROR: symbol \"%s\" could not be resolved.\n", identifier);
				fflush(stdout);
				fflush(file);
				system("PAUSE");
				exit(5);
			}
		}
	}
	else {
		if(!strcmp(identifier, "")) {
			// TODO: attribution operation when the second operand is an constant
		}
		else {
			i = lookUpForCell(*symbols, identifier, "variable");

			if(i >= 0) {
				if(!strcmp(operator, "+")) {
					fprintf(file, "%s\t +\t%s\n", label, identifier);
				}
				else if(!strcmp(operator, "-")) {
					fprintf(file, "%s\t -\t%s\n", label, identifier);
				}
				else if(!strcmp(operator, "*")) {
					fprintf(file, "%s\t *\t%s\n", label, identifier);
				}
				else if(!strcmp(operator, "/")) {
					fprintf(file, "%s\t /\t%s\n", label, identifier);
				}
				fprintf(file, "\tMM\t%s\n", destination);
				fflush(file);
			}
			else {
				printf("ERROR: symbol \"%s\" could not be resolved.\n", identifier);
				fprintf(file, "ERROR: symbol \"%s\" could not be resolved.\n", identifier);
				fflush(stdout);
				fflush(file);
				system("PAUSE");
				exit(5);
			}
		}
	}
	constant = -1;
	strcpy(identifier, "");
	strcpy(operator, "");
	strcpy(destination, "");
	strcpy(label, "");
}

