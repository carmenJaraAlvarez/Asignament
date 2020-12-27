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
Type get_type(PAproblemPD appd){
	  Type res;
	  res=getAproblemType(appd->aproblem);
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
	  return res;

  }
 //check resource with index i, is an option
 Logico is_alternative(PAproblemPD papd, int i){
	 Logico res=TRUE;
	 //if it is the first assignment, every resource is free
	 if(papd->index==0){
		 return res;
	 }
	 else{//if it is not the first time>>if the resource is in previous solution, it is not valid.
		 for(int j=0;j<papd->solution.lengthArrays;j++){
			 if(papd->solution.resources[j].name==papd->aproblem.resources[i].name){
			 res=FALSE;
			 break;
			 }
		 }
	 }
	 return res;
 }
 int select_alternative(PAproblemPD papd,ArrayAlternatives as,int numAlternatives, double* selectedValue ){
	 int selected=-1;
	 double bestValue;
	 double actualValue;
	 if(get_type(papd)==0){//MAX
		 bestValue=-999999.;
	 }
	 if(get_type(papd)==1){//MIN
		 bestValue=999999.;
	 }
	 //get values array
	  int indexValue=papd->index*papd->aproblem.numResources;//where we begin
	  for(int i=0;i<numAlternatives;i++){

		  actualValue=papd->aproblem.values[indexValue+as[i].indexResource];
		  if(get_type(papd)==0 && actualValue>bestValue){//MAX
			  bestValue=actualValue;
			  selected=as[i].indexResource;
		  }
		  else if(get_type(papd)==1 && actualValue<bestValue){
			  bestValue=actualValue;
			  selected=as[i].indexResource;
		  }
	  }

	  *selectedValue=bestValue;
	  return selected;
  }
  int get_solution_base_case(PAproblemPD papd, PSpPD psp){
	  int res=0;
	  int numAlternatives=0;
	  double selectedValue;
	  //get array alternatives (a=estruct with int)
	  ArrayAlternatives alternatives;
	  numAlternatives=get_alternatives(papd, alternatives);
	  //select resource max or min value, return resource index and its value
	  int alternative=select_alternative(papd, alternatives, numAlternatives, &selectedValue);
	  psp->alternative.indexResource=alternative;
	  psp->value=selectedValue;
	  return res;
  }

  int combine_solutions(AproblemPD appd, PSolution ps, SpPD sp ){
	  int res=0;
	  ps->acum+=sp.value;
	  ps->lengthArrays+=1;
	  strcpy( ps->resources[ ps->lengthArrays-1].name,appd.aproblem.resources[sp.alternative.indexResource].name);
	  return res;

  }
  int get_num_subproblems(){
	  return 1;
  }
  int get_subproblem(const PAproblemPD  father, PAproblemPD new,Alternative a, int numSubproblems){
	  int res=0;
	  double value;
	  printf("inside get_subproblem");
	  new->aproblem=father->aproblem;

	  //update
	  value=father->aproblem.values[(father->index)*(father->aproblem.numResources)+a.indexResource];
	  new->index+=1;
	  new->solution=father->solution;
	  updateSolution(&(new->solution), &a, value);
	  return res;
  }
