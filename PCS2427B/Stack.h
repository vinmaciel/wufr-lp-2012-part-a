/*
 * Stack.h
 *
 *  Created on: 05/12/2012
 *      Author: Vinicius
 */

#ifndef STACK_H_
#define STACK_H_

typedef struct _StackNode {
	int index;
	struct _StackNode* next;
} StackNode;

typedef StackNode* Stack;

Stack simpleFreeList;

void newFreeList(void);
void newNode();
void releaseNode(Stack freeNode);
void getNewNode(Stack* newNode);

void newStack(Stack* stack);
void cleanStack(Stack* stack);
void push(Stack* stack, int element);
int pop(Stack* stack);
int top(Stack stack);

#endif /* STACK_H_ */
