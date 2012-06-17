/*
 * AutomataStack.c
 *
 *  Created on: 17/06/2012
 *      Author: Vinicius
 */

#include <stdlib.h>
#include "AutomataStack.h"

void newAutomataStack(AutomataStack* stack) {
	*stack = NULL;
	freeList = NULL;
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

	if(*stack == NULL) {
		newNode->next = newNode;
		*stack = newNode;
	}
	else {
		newNode->next = (*stack)->next;
		(*stack)->next = newNode;
	}
}

void popAutomaton(AutomataStack* stack, int* machine, int* nextState) {
	AutomataStack freeNode;

	freeNode = (*stack)->next;

	*machine = freeNode->machineIndex;
	*nextState = freeNode->nextStateIndex;

	if(*stack == (*stack)->next)
		*stack = NULL;
	else
		(*stack)->next = freeNode->next;

	releaseStackNode(freeNode);
}

int isEmptyStack(AutomataStack stack) {
	if(stack == NULL)
		return 1;

	return 0;
}
