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
int getToken(StringManager* manager, char* token);
void recycleToken(StringManager* manager, char* token);
char* printString(StringManager manager);
char* printToken(StringManager manager, int tokenSize);

#endif /* STRINGMANAGER_H_ */
