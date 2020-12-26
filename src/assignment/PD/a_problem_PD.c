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
  //return num alternatives and modified array of alternatives with index resources
  int get_alternatives(PAproblemPD papd, ArrayAlternatives as){
	  int res=0;
	  int numResources=papd->aproblem.numResources;
	  ArrayAlternatives aux;
	  for(int i=0;i<numResources;i++){//for every resource
		  if(is_alternative(papd,i)){//filter
			  Alternative a;
			  initAlternative(&a,i);
			  aux[res]=a;
			  res++;
		  }
	  }
	  for(int i=0;i<res;i++){
		  as[i]=aux[i];
	  }
	  //*pas=aux;//not seen in father function
	  return res;

  }
 //check resource with index i, is an option
 Logico is_alternative(PAproblemPD papd, int i){
	 Logico res=TRUE;
	 //TODO
	 return res;
 }
 int select_alternative(PAproblemPD papd,ArrayAlternatives as,int numAlternatives, double* selectedValue ){
	 int selected=-1;
	 double max=-9999.;//TODO
	 //get values array
	  int indexValue=papd->index*papd->aproblem.numResources;//where we begin
	  for(int i=0;i<numAlternatives;i++){

		  double actualValue=papd->aproblem.values[indexValue+as[i].indexResource];
		  if(actualValue>max){
			  max=actualValue;
			  selected=as[i].indexResource;
		  }
	  }

	  *selectedValue=max;
	  return selected;
  }
  int get_solution_base_case(PAproblemPD papd){
	  int res=0;
	  int numAlternatives=0;
	  double selectedValue;
	  //get array alternatives (a=estruct with int)
	  ArrayAlternatives alternatives;
	  numAlternatives=get_alternatives(papd, alternatives);
	  //select resource max or min value
	  int alternative=select_alternative(papd, alternatives, numAlternatives, &selectedValue);
	  //solution.acum+=selected value
	  papd->solution.acum+=selectedValue;
	  papd->solution.lengthArrays+=1;
	  Cadena resourceName;
	  //TODO
//	  strcpy(resourceName,papd->aproblem.resources[alternative]->name);
	  strcpy(papd->solution.resources[(papd->solution.lengthArrays)-1].name,resourceName);

	  return res;
  }
  int get_subproblem(const PAproblemPD  father, PAproblemPD new,Alternative a){
	  int res=0;
	  printf("inside get_subproblem");
	  return res;
  }

