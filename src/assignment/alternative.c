/*
 * alternative.c
 *
 *  Created on: 24 dic. 2020
 *      Author: practica
 */
#include "alternative.h"

int init_alternative(PAlternative a,const int i)
{
	int res=0;
	a->indexResource=i;
	return res;
}
int init_alternative_array(Alternative ** a,int max)
{
	int res=0;
	*a=(Alternative*)malloc(sizeof(Alternative)*max);
	return res;
}
int delete_alternatives(PAlternative* pa)
{
	int res=0;
//	free(pa);
	return res;
}
