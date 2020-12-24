/*
 * solution.c
 *
 *  Created on: 24 dic. 2020
 *      Author: practica
 */

#include "solution.h"

int solutionToString(PSolution ps){
	int res=0;
	printf("Solution: %f\n",ps->acum);
	int l=ps->lengthArrays;
	for(int i=0;i<l; i++){
		printf("task %s - resource %s\n", ps->tasks[i]->name, ps->resources[i]->name);
	}
	return res;
}
