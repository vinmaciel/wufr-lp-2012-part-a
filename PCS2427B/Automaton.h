/*
 * Graph.h
 *
 *  Created on: 09/05/2012
 *      Author: Leonardo Pedro Kakitani
 *      		Marcos Vinicius Maciel da Silva
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include "Table.h"
#include "StringManager.h"

typedef struct {
	Table symbolTable;
	Table stateTable;
	int** production;	// production[current_state][symbol] => next_state
	int* submachine[2];	// submachine[current_state] => (next_machine, next_state_after_return)
} AutomatonStruct;

typedef AutomatonStruct* Automaton;

void createGraph(FILE* input, Automaton* automaton, Table automataTable);
void createAutomaton(FILE* input, Automaton* automaton);
void generateTokenFile(Automaton lexer, const char* inputFileName);

#endif /* GRAPH_H_ */
