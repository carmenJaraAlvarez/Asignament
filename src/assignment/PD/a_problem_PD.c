/*
 * a_problem_PD.c
 *
 *  Created on: 22 dic. 2020
 *      Author: practica
 */
#include "a_problem_PD.h"

int initAProblemPD(PAproblemPD papd,Aproblem pa){
	  int res=0;
	  papd->index=0;
	  papd->aproblem=pa;
	  return res;
}
  Logico is_base_case(PAproblemPD papd){
	  Logico res=FALSE;
	  if(papd->index==(papd->aproblem.numTask)-1){
		  res=TRUE;
	  }
	  return res;
  }
  int get_alternatives(PAproblemPD papd, ArrayPAlternatives as){
	  int res=0;
	  int numResources=papd->aproblem.numResources;
	  for(int i=0;i<numResources;i++){
		  Alternative a;
		  initAlternative(&a,((papd->index)*numResources+i));
		  as[i]=&a;//TODO is not working, is the same direction
	  }
	  return res;
  }
 int select_alternative(PAproblemPD papd,ArrayPAlternatives as,double* selectedValue ){
	 int selected=-1;
	 double min=-9999.;
	  selectedValue=&min;//TODO
	  //where we are in values array
	  int indexLast=papd->index*papd->aproblem.numResources;
	  for(int i=0;i<papd->aproblem.numResources;i++){
		  int indexNow=indexLast+as[i]->indexResource;
		  double actualValue=papd->aproblem.values[indexNow];
		  if(actualValue>*selectedValue){
			  selectedValue=&actualValue;
			  selected=as[i]->indexResource;
		  }
	  }
	  return selected;
  }
  int get_solution_base_case(PAproblemPD papd, PSolution ps){
	  int res=0;
	  //get array alternatives (a=estruct with int)
	  ArrayPAlternatives alternatives;
	  get_alternatives(papd, alternatives);
	  //select resource max or min value
	  double selectedValue;
	  int alternative=select_alternative(papd, alternatives, &selectedValue);
	  //solution.acum+=selected value
	  papd->solution.acum+=selectedValue;
	  //solution.resurces+=selected resource
	  Cadena resourceName;
	  //TODO
//	  strcpy(resourceName,papd->aproblem.resources[alternative]->name);

	  papd->solution.lengthArrays+=1;
	  strcpy(papd->solution.resources[papd->solution.lengthArrays]->name,
			  resourceName);

	  return res;
  }
  int get_subproblem(const PAproblemPD  father, PAproblemPD new,Alternative a){
	  int res=0;
	  printf("inside get_subproblem");
	  return res;
  }

