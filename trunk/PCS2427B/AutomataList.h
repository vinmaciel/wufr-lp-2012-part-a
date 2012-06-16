/*
 * AutomataList.h
 *
 *  Created on: 15/06/2012
 *      Author: Vinicius
 */

#ifndef AUTOMATALIST_H_
#define AUTOMATALIST_H_

#include "Automaton.h"

typedef struct _AutomataListNode {
	Automaton elem;
	struct _AutomataListNode* next;
} AutomataListNode;

typedef AutomataListNode* AutomataList;

void newAutomataList(AutomataList* list);
void addAutomatonToList(AutomataList* list, Automaton automaton);
Automaton getAutomatonByIndex(AutomataList list, int index);

#endif /* AUTOMATALIST_H_ */
