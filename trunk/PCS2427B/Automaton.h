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
#include "Lexer.h"

typedef struct {
	Table tokenTable;
	Table stateTable;
	int** production;	// production[current_state][token] => next_state
	int* submachine[2];	// submachine[current_state] => (next_machine, next_state_after_return)
} AutomatonStruct;

typedef AutomatonStruct* Automaton;

void createGraph(FILE* input, Automaton* automaton, Table automataTable);

#endif /* GRAPH_H_ */
