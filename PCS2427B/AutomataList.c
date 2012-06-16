/*
 * AutomataList.c
 *
 *  Created on: 15/06/2012
 *      Author: Vinicius
 */

#include <stdlib.h>
#include "Automaton.h"
#include "AutomataList.h"

void newAutomataList(AutomataList* list) {
	*list = NULL;
}

// add to the end of the list
void addAutomatonToList(AutomataList* list, Automaton automaton) {
	AutomataList newNode, search;

	newNode = (AutomataListNode*) malloc(sizeof(AutomataListNode));
	newNode->elem = automaton;
	newNode->next = NULL;

	if(*list == NULL)
		*list = newNode;
	else {
		for(search = *list; search->next != NULL; search = search->next);
		search->next = newNode;
	}
}

Automaton getAutomatonByIndex(AutomataList list, int index) {
	AutomataList search;
	int i;

	for(i = 0, search = list; i < index && search != NULL; i++, search = search->next);

	return search->elem;
}
