/*
 * machine.h
 *
 *  Created on: 15/06/2012
 *      Author: Vinicius
 */

#ifndef MACHINE_H_
#define MACHINE_H_

#include "Table.h"
#include "AutomataList.h"

typedef struct {
	Table automataTable;
	AutomataList automataList;
} MachineStruct;

typedef MachineStruct* Machine;

void createMachine(FILE* input, Machine* machine);
int testString(Machine machine, StringManager* stringManager);
int consumeFile(Automaton lexer, Machine parser, Table keywords, const char* inputFileName);

#endif /* MACHINE_H_ */
