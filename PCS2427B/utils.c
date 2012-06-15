/*
 * utils.c
 *
 *  Created on: 15/06/2012
 *      Author: Vinicius
 */

#include <ctype.h>
#include "utils.h"

void cleanString(char* string) {
	int i;

	for(i = 0; string[i] != '\0'; i++) {
		if(iscntrl(string[i])) {
			string[i] = '\0';
			i--;
		}
	}
}
