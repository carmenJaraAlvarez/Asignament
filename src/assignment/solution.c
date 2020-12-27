/*
 * solution.c
 *
 *  Created on: 24 dic. 2020
 *      Author: practica
 */

#include "solution.h"

int init_solution(PSolution ps){
	ps->acum=0.;
	ps->lengthArrays=0;
}

int solutionToString(Solution s){
	int res=0;
	printf("Solution: %f\n",s.acum);
	int l=s.lengthArrays;
	for(int i=0;i<l; i++){
		printf("task %d - resource %s\n", i, s.resources[i].name);
	}
	return res;
}
int updateSolution(PSolution ps, PAlternative pa, double value){
	int res=0;
	ps->lengthArrays=ps->lengthArrays+1;
	ps->acum=(ps->acum)+value;
	Resource resource;
	init_resource(&resource,"test");//TODO
	ps->resources[(ps->lengthArrays)-1]=resource;
	return res;
}
