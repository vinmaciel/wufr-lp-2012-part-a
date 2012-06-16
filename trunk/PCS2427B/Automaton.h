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
	int** production;
} AutomatonStruct;

typedef AutomatonStruct* Automaton;

void createGraph(FILE* input, Automaton* automaton);

#endif /* GRAPH_H_ */