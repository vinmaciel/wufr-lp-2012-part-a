/*
 * AutomataStack.h
 *
 *  Created on: 17/06/2012
 *      Author: Vinicius
 */

#ifndef AUTOMATASTACK_H_
#define AUTOMATASTACK_H_

#include "Automaton.h"

typedef struct _AutomataStackNode {
	int machineIndex;
	int nextStateIndex;
	struct _AutomataStackNode* next;
} AutomataStackNode;

typedef AutomataStackNode* AutomataStack;

AutomataStack freeList;

void newFreeList(void);
void newAutomataStack(AutomataStack* stack);
void getNewStackNode(AutomataStack* newNode);
void releaseStackNode(AutomataStack freeNode);
void pushAutomaton(AutomataStack* stack, int machine, int nextState);
void popAutomaton(AutomataStack* stack, int* machine, int* nextState);
int isEmptyStack(AutomataStack stack);

#endif /* AUTOMATASTACK_H_ */
