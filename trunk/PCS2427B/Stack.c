/*
 * Stack.c
 *
 *  Created on: 05/12/2012
 *      Author: Vinicius
 */

#include "Stack.h"
#include <stdlib.h>

void newFreeList(void) {
	simpleFreeList = NULL;
}

void releaseNode(Stack freeNode) {
	freeNode->next = simpleFreeList;
	simpleFreeList = freeNode;
}

void newStack(Stack* stack) {
	*stack = NULL;
}

void getNewNode(Stack* newNode) {
	if(simpleFreeList == NULL)
		*newNode = (StackNode*) malloc(sizeof(StackNode));
	else {
		*newNode = simpleFreeList;
		simpleFreeList = simpleFreeList->next;
	}
}

void cleanStack(Stack* stack) {
	Stack search, freeNode;

	search = *stack;
	while(search != NULL) {
		freeNode = search;
		search = search->next;

		releaseNode(freeNode);
	}
}

void push(Stack* stack, int element) {
	Stack newNode;

	getNewNode(&newNode);
	newNode->index = element;
	newNode->next = *stack;
	*stack = newNode;
}

int pop(Stack* stack) {
	Stack top;
	int index;

	top = *stack;
	*stack = (*stack)->next;
	index = top->index;
	releaseNode(top);

	return index;
}

int top(Stack stack) {
	return stack->index;
}
