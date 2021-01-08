/*
 * solution.c
 *
 *  Created on: 24 dic. 2020
 *      Author: practica
 */

#include "solution.h"

int init_solution(PSolution ps,int maxLength)
{
	int res=0;
	ps->acum=0.;
	ps->lengthArrays=0;
	ps->resources=(Resource*)malloc(sizeof(Resource)*maxLength);
	return res;
}
int delete_solution(PSolution ps)
{
	int res=0;
	free(ps->resources);
	return res;
}

int show_solution(const Solution s)
{
	int res=0;
	printf("Solution: %lf\n",s.acum);
	int l=s.lengthArrays;
	for(int i=0;i<l; i++)
	{
		printf("task %d - resource %s\n", i, s.resources[i].name);
	}
	return res;
}

