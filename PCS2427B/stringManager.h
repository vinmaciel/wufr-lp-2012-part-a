/*
 * stringManager.h
 *
 *  Created on: 14/06/2012
 *      Author: Vinicius
 */

#ifndef STRINGMANAGER_H_
#define STRINGMANAGER_H_

#include <stdio.h>

typedef struct {
	char* string;
	int index;
} StringManagerStruct;

typedef StringManagerStruct* StringManager;

void startStringManager(StringManager* manager);
void getStringFromConsole(StringManager* manager);
void getStringFromFile(StringManager* manager, FILE* file);
int getSymbol(StringManager* manager, char* symbol);
void recycleSymbol(StringManager* manager, char* symbol);
char* printString(StringManager manager);
char* printSymbol(StringManager manager, int symbolSize);

#endif /* STRINGMANAGER_H_ */
