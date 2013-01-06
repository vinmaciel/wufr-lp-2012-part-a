/*
 * Semantic.c
 *
 *  Created on: 16/11/2012
 *      Author: Vinicius
 */

#include <string.h>
#include <stdlib.h>
#include "Semantics.h"
#include "Stack.h"
#include "Token.h"
#include "Table.h"


/******** PRIVATE PARAMETERS ********/
char identifier[64];
int constant;
char procedure[64];
char label[64];
char type[5];
char destination[64];
char operator[2];
Stack ifs;
int ifCount;
Stack whiles;
int whileCount;

char auxiliar[64];
/************************************/

void startSemantics(FILE* file, DynamicTable* symbols) {
	strcpy(identifier, "");
	strcpy(label, "");
	strcpy(procedure, "");
	strcpy(type, "");
	strcpy(destination, "");
	strcpy(operator, "");
	strcpy(auxiliar, "");

	constant = -1;
	ifCount = -1;
	whileCount = -1;

	newFreeList();
	newStack(&ifs);
	newStack(&whiles);

	fprintf(file, "_start\tSC\tmain\n\tHM\t_start\n\n");
	addToTable(symbols, "main", "procedure");
}

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
	if(!strcmp(label, "setConditional"))
		return 12;
	if(!strcmp(label, "endConditional"))
		return 13;
	if(!strcmp(label, "noElse"))
		return 14;
	if(!strcmp(label, "endElse"))
		return 15;
	if(!strcmp(label, "setIteration"))
		return 16;
	if(!strcmp(label, "setLoop"))
		return 17;
	if(!strcmp(label, "endIteration"))
		return 18;
	if(!strcmp(label, "setLink"))
		return 19;
	if(!strcmp(label, "setProcedure"))
		return 20;

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
	case 12:
		return setConditional;
	case 13:
		return endConditional;
	case 14:
		return noElse;
	case 15:
		return endElse;
	case 16:
		return setIteration;
	case 17:
		return setLoop;
	case 18:
		return endIteration;
	case 19:
		return setLink;
	case 20:
		return setProcedure;
	}
	return nil;
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
	// allows end of conditional or iteration blocks label with NOP (instruction with no effect)
	if(!strncmp(label, "_else", 5)) {
		fprintf(file, "%s\tJP\t_then%d\n", label, top(ifs));
		strcpy(label, "_then");
		strcat(label, integerToString(auxiliar, pop(&ifs), 10));

		strcpy(auxiliar, "");
	}
	if(!strncmp(label, "_then", 5)) {
		fprintf(file, "%s\t +\t_nil\n", label);
		strcpy(label, "");
	}
	if(!strncmp(label, "_finally", 8)) {
		fprintf(file, "%s\t +\t_nil\n", label);
		strcpy(label, "");
	}
	fflush(file);

	// cannot end block with opened label
	if(strcmp(label, "")) {
		printf("ERROR: label \"%s\" at the end of compound statement.\n", label);
		fprintf(file, "ERROR: label \"%s\" at the end of compound statement.\n", label);
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}

	fprintf(file, "\n");
}
// 5
void endFile(FILE* file, DynamicTable* symbols, Token token) {
	DynamicTable search;
	int i;

	i = lookUpForCell(*symbols, "main", "procedure");
	if(i == -1) {
		printf("ERROR: unexpected behavior defining main procedure.\n");
		fprintf(file, "ERROR: unexpected behavior defining main procedure.\n");
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}
	else {
		if(!getRow(*symbols, i)->defined) {
			printf("WARNING: main function not defined.\n");
			fflush(stdout);
			defineRow(symbols, i);
		}
	}

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

	// constant: 0
	fprintf(file, "_nil\t K\t/0000\n");
	// constant: -1
	fprintf(file, "_inv\t K\t/FFFF\n");

	// EOF
	fprintf(file, "\t#\t_start\n");
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
// 20
void setProcedure(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: procedure id\n");fflush(stdout);
	strcpy(procedure, token->value);
}
// 19
void setLink(FILE* file, DynamicTable* symbols, Token token) {
	int i;

	printf("semantic: procedure\n");fflush(stdout);
	// TODO: procedures and variables still cannot have the same name
	i = lookUpForCell(*symbols, procedure, "procedure");
	if(i == -1) {
		i = addToTable(symbols, procedure, "procedure");
		defineRow(symbols, i);
		fprintf(file, "%s\t K\t/0000\n", procedure);
		fflush(file);
		strcpy(procedure, "");
	}
	else if(!strcmp(procedure, "main") && !getRow(*symbols, i)->defined) {
		defineRow(symbols, i);
	}
	else {
		printf("ERROR: redeclaration of \"%s\".\n", procedure);
		fprintf(file, "ERROR: redeclaration of \"%s\".\n", procedure);
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}
}

/******** LABEL ********/
// 2
void setLabel(FILE* file, DynamicTable* symbols, Token token) {
	int i;

	printf("semantic: label\n");fflush(stdout);
	// if instruction already has a label, jump to the last assigned label
	if(strcmp(label,"")) {
		i = lookUpForCell(*symbols, identifier, "label");
		if(i == -1) {
			i = addToTable(symbols, identifier, "label");
			defineRow(symbols, i);
			fprintf(file, "%s\tJP\t%s\n", identifier, label);
			fflush(file);
			strcpy(identifier, "");
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
// 12
void setConditional(FILE* file, DynamicTable* symbols, Token token) {
	int i;

	printf("semantic: conditional ");fflush(stdout);
	ifCount++;
	push(&ifs, ifCount);

	if(!strcmp(operator, "")) {
		printf("with one operand\n");fflush(stdout);
		if(!strcmp(identifier, "")) {
			fprintf(file, "%s\tLV\t/%04X\n", label, constant);
			fprintf(file, "\tJZ\t_else%d\n", top(ifs));
			fflush(file);

			strcpy(label, "_if");
			strcat(label, integerToString(auxiliar, top(ifs), 10));

			constant = -1;
			strcpy(auxiliar, "");
		}
		else {
			i = lookUpForCell(*symbols, identifier, "variable");

			if(i >= 0) {
				fprintf(file, "%s\tLD\t%s\n", label, identifier);
				fprintf(file, "\tJZ\t_else%d\n", top(ifs));
				fflush(file);

				strcpy(label, "_if");
				strcat(label, integerToString(auxiliar, top(ifs), 10));

				strcpy(identifier, "");
				strcpy(auxiliar, "");
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
			printf("not implemented\n");fflush(stdout);
			// TODO: comparison operation when the second operand is an constant
		}
		else {
			i = lookUpForCell(*symbols, identifier, "variable");

			if(i >= 0) {
				fprintf(file, "\t -\t%s\n", identifier);
				if(!strcmp(operator, "==")) {
					printf("with equals\n");fflush(stdout);
					fprintf(file, "\tJZ\t_if%d\n", top(ifs));
					fprintf(file, "\tJP\t_else%d\n", top(ifs));
				}
				else if(!strcmp(operator, "<")) {
					printf("with lower than\n");fflush(stdout);
					fprintf(file, "\tJN\t_if%d\n", top(ifs));
					fprintf(file, "\tJP\t_else%d\n", top(ifs));
				}
				else if(!strcmp(operator, ">")) {
					printf("with greater than\n");fflush(stdout);
					fprintf(file, "\t *\t_inv\n");
					fprintf(file, "\tJN\t_if%d\n", top(ifs));
					fprintf(file, "\tJP\t_else%d\n", top(ifs));
				}
				fflush(file);

				strcpy(label, "_if");
				strcat(label, integerToString(auxiliar, top(ifs), 10));

				strcpy(identifier, "");
				strcpy(operator, "");
				strcpy(auxiliar, "");
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
}
// 13
void endConditional(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: EOC\n");fflush(stdout);
	// allows end of conditional or iteration blocks label with NOP (instruction with no effect)
	if(!strncmp(label, "_else", 5)) {
		fprintf(file, "%s\tJP\t_then%d\n", label, top(ifs));
		strcpy(label, "_then");
		strcat(label, integerToString(auxiliar, pop(&ifs), 10));

		strcpy(auxiliar, "");
	}
	fflush(file);

	if(strcmp(label, "") && strncmp(label, "_then", 5) && strncmp(label, "_finally", 8)) {
		// allows end of iteration block label with NOP (instruction with no effect)
		printf("ERROR: label \"%s\" at the end of compound statement.\n", label);
		fprintf(file, "ERROR: label \"%s\" at the end of compound statement.\n", label);
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}
	fprintf(file, "%s\tJP\t_then%d\n", label, top(ifs));
	fflush(file);

	strcpy(label, "_else");
	strcat(label, integerToString(auxiliar, top(ifs), 10));

	strcpy(auxiliar, "");
}
// 14
void noElse(FILE* file, DynamicTable* symbols, Token token) {
	fprintf(file, "%s\tJP\t_then%d\n", label, top(ifs));
	fflush(file);

	strcpy(label, "_then");
	strcat(label, integerToString(auxiliar, pop(&ifs), 10));

	strcpy(auxiliar, "");

	if(!strcmp(token->type, "IDENTIFIER"))
		setIdentifier(file, symbols, token);
}
// 15
void endElse(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: EOE\n");fflush(stdout);
	// allows end of conditional or iteration blocks label with NOP (instruction with no effect)
	if(!strncmp(label, "_else", 5)) {
		fprintf(file, "%s\tJP\t_then%d\n", label, top(ifs));
		strcpy(label, "_then");
		strcat(label, integerToString(auxiliar, pop(&ifs), 10));

		strcpy(auxiliar, "");
	}
	fflush(file);

	if(strcmp(label, "") && strncmp(label, "_then", 5) && strncmp(label, "_finally", 8)) {
		printf("ERROR: label \"%s\" at the end of compound statement.\n", label);
		fprintf(file, "ERROR: label \"%s\" at the end of compound statement.\n", label);
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}

	fprintf(file, "%s\tJP\t_then%d\n", label, top(ifs));
	strcpy(label, "_then");
	strcat(label, integerToString(auxiliar, pop(&ifs), 10));

	strcpy(auxiliar, "");
}

/******** LOOP ********/
// 17
void setLoop(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: loop\n");fflush(stdout);
	whileCount++;
	push(&whiles, whileCount);

	if(strcmp(label, "")) {
		fprintf(file, "%s\tJP\t_while%d\n", label, top(whiles));
	}
	strcpy(label, "_while");
	strcat(label, integerToString(auxiliar, top(whiles), 10));

	strcpy(auxiliar, "");
}
// 16
void setIteration(FILE* file, DynamicTable* symbols, Token token) {
	int i;

	printf("semantic: iteration ");fflush(stdout);

	if(!strcmp(operator, "")) {
		printf("with one operand\n");fflush(stdout);
		if(!strcmp(identifier, "")) {
			fprintf(file, "%s\tLV\t/%04X\n", label, constant);
			fprintf(file, "\tJZ\t_finally%d\n", top(whiles));
			fflush(file);

			strcpy(label, "_do");
			strcat(label, integerToString(auxiliar, top(whiles), 10));

			constant = -1;
			strcpy(auxiliar, "");
		}
		else {
			i = lookUpForCell(*symbols, identifier, "variable");

			if(i >= 0) {
				fprintf(file, "%s\tLD\t%s\n", label, identifier);
				fprintf(file, "\tJZ\t_finally%d\n", top(whiles));
				fflush(file);

				strcpy(label, "_do");
				strcat(label, integerToString(auxiliar, top(whiles), 10));

				strcpy(identifier, "");
				strcpy(auxiliar, "");
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
			printf("not implemented\n");fflush(stdout);
			// TODO: comparison operation when the second operand is an constant
		}
		else {
			i = lookUpForCell(*symbols, identifier, "variable");

			if(i >= 0) {
				fprintf(file, "\t -\t%s\n", identifier);
				if(!strcmp(operator, "==")) {
					printf("with equals\n");fflush(stdout);
					fprintf(file, "\tJZ\t_do%d\n", top(whiles));
					fprintf(file, "\tJP\t_finally%d\n", top(whiles));
				}
				else if(!strcmp(operator, "<")) {
					printf("with lower than\n");fflush(stdout);
					fprintf(file, "\tJN\t_do%d\n", top(whiles));
					fprintf(file, "\tJP\t_finally%d\n", top(whiles));
				}
				else if(!strcmp(operator, ">")) {
					printf("with greater than\n");fflush(stdout);
					fprintf(file, "\t *\t_inv\n");
					fprintf(file, "\tJN\t_do%d\n", top(whiles));
					fprintf(file, "\tJP\t_finally%d\n", top(whiles));
				}
				fflush(file);

				strcpy(label, "_do");
				strcat(label, integerToString(auxiliar, top(whiles), 10));

				strcpy(identifier, "");
				strcpy(operator, "");
				strcpy(auxiliar, "");
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
}
// 18
void endIteration(FILE* file, DynamicTable* symbols, Token token) {
	printf("semantic: EOI\n");fflush(stdout);
	// allows end of conditional or iteration blocks label with NOP (instruction with no effect)
	if(!strncmp(label, "_else", 5)) {
		fprintf(file, "%s\tJP\t_then%d\n", label, top(ifs));
		strcpy(label, "_then");
		strcat(label, integerToString(auxiliar, pop(&ifs), 10));

		strcpy(auxiliar, "");
	}
	fflush(file);

	// cannot end block with opened label
	if(strcmp(label, "") && strncmp(label, "_then", 5) && strncmp(label, "_do", 3) && strncmp(label, "_finally", 8)) {
		printf("ERROR: label \"%s\" at the end of compound statement.\n", label);
		fprintf(file, "ERROR: label \"%s\" at the end of compound statement.\n", label);
		fflush(stdout);
		fflush(file);
		system("PAUSE");
		exit(5);
	}

	fprintf(file, "%s\tJP\t_while%d\n", label, top(whiles));
	fflush(file);

	strcpy(label, "_finally");
	strcat(label, integerToString(auxiliar, pop(&whiles), 10));

	strcpy(auxiliar, "");
}

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

	printf("semantic: attribution ");fflush(stdout);

	if(!strcmp(operator, "")) {
		printf("with one operand\n");fflush(stdout);
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
			printf("not implemented\n");fflush(stdout);
			// TODO: attribution operation when the second operand is an constant
		}
		else {
			i = lookUpForCell(*symbols, identifier, "variable");

			if(i >= 0) {
				if(!strcmp(operator, "+")) {
					printf("with add\n");fflush(stdout);
					fprintf(file, "%s\t +\t%s\n", label, identifier);
				}
				else if(!strcmp(operator, "-")) {
					printf("with sub\n");fflush(stdout);
					fprintf(file, "%s\t -\t%s\n", label, identifier);
				}
				else if(!strcmp(operator, "*")) {
					printf("with mult\n");fflush(stdout);
					fprintf(file, "%s\t *\t%s\n", label, identifier);
				}
				else if(!strcmp(operator, "/")) {
					printf("with div\n");fflush(stdout);
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

