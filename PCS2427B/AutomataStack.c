/*
 * AutomataStack.c
 *
 *  Created on: 17/06/2012
 *      Author: Vinicius
 */

#include <stdlib.h>
#include "AutomataStack.h"

void newFreeList(void) {
	freeList = NULL;
}

void newAutomataStack(AutomataStack* stack) {
	*stack = NULL;
}

void getNewStackNode(AutomataStack* newNode) {
	if(freeList == NULL)
		*newNode = (AutomataStackNode*) malloc(sizeof(AutomataStackNode));
	else {
		*newNode = freeList;
		freeList = freeList->next;
	}
}

void releaseStackNode(AutomataStack freeNode) {
	freeNode->next = freeList;
	freeList = freeNode;
}

void pushAutomaton(AutomataStack* stack, int machine, int nextState) {
	AutomataStack newNode;

	getNewStackNode(&newNode);
	newNode->machineIndex = machine;
	newNode->nextStateIndex = nextState;

	newNode->next = *stack;
	*stack = newNode;
}

void popAutomaton(AutomataStack* stack, int* machine, int* nextState) {
	AutomataStack freeNode;

	freeNode = *stack;

	*machine = freeNode->machineIndex;
	*nextState = freeNode->nextStateIndex;

	*stack = (*stack)->next;

	releaseStackNode(freeNode);
}

int isEmptyStack(AutomataStack stack) {
	if(stack == NULL)
		return 1;

	return 0;
}
