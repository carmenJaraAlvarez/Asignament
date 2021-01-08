/*
 * a_problem_PD.c
 *
 *  Created on: 22 dic. 2020
 *      Author: practica
 */
#include "a_problem_PD.h"

int initAProblemPD(PAproblemPD papd,PAproblem pa)
{
	  int res=0;
	  papd->index=0;
	  papd->aproblem=*pa;
	  Solution sol;
	  init_solution(&sol,pa->numTask);
	  papd->solution=sol;
	  return res;
}

Type get_type(PAproblemPD appd)
{
	  Type res;
	  res=get_aproblem_type(&(appd->aproblem));
	  return res;
}

  Logico is_base_case(const PAproblemPD papd)
  {
	  Logico res=FALSE;
	  if(papd->index==(papd->aproblem.numTask)-1){
		  res=TRUE;
	  }
	  return res;
  }

  //return num alternatives and modified array of alternatives with index resources
  int get_alternatives(const PAproblemPD papd, PAlternative as)
  {
	  int res=0;
	  int numResources=papd->aproblem.numResources;
	  printf("\n Inside get alternative. In: ");
	  for(int j=0;j<papd->solution.lengthArrays;j++ )
	  {
		  printf("%s ", papd->solution.resources[j].name);
	  }
	  printf("\n");

	 Alternative aux[get_max_num_alternatives(&(papd->aproblem))];

	  for(int i=0;i<numResources;i++)//for every resource
	  {
		  if(is_alternative(papd,i))//filter
		  {
			  Alternative a;
			  init_alternative(&a,i);
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
 Logico is_alternative(const PAproblemPD papd, int i)
 {
	 Logico res=TRUE;
	 //if it is the first assignment, every resource is free
	 if(papd->index==0)
	 {
		 return res;
	 }
	 else//if it is not the first time>>if the resource is in previous solution, it is not valid.
	 {
		 for(int j=0;j<papd->solution.lengthArrays;j++)
		 {
			 if(strcmp(papd->solution.resources[j].name,papd->aproblem.resources[i].name)==0)
			 {
			 res=FALSE;
			 break;
			 }
		 }
	 }
	 return res;
 }
 int select_alternative(PAproblemPD papd,PAlternative as,int numAlternatives, double* selectedValue ){
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
		  int t=get_type(papd);
		  if(get_type(papd)==0 && actualValue>bestValue)//MAX
		  {
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
  int get_solution_base_case(PAproblemPD papd, PSpPD psp)
  {
	  int res=0;
	  int numAlternatives=0;
	  double selectedValue;
	  //get array alternatives (a=estruct with int)
	  PAlternative alternatives;
	  init_alternative_array(&alternatives,50);//TODO MAX
	  numAlternatives=get_alternatives(papd, alternatives);
	  //select resource max or min value, return resource index and its value
	  int alternative=select_alternative(papd, alternatives, numAlternatives, &selectedValue);
	  psp->alternative.indexResource=alternative;
	  psp->value=selectedValue;
	  //**************

	  papd->solution.acum+=psp->value;
	  papd->solution.lengthArrays++;
	  Resource resource;
	  init_resource(&resource,papd->aproblem.resources[psp->alternative.indexResource].name);
	  papd->solution.resources[papd->index]=resource;
//	  strcpy(papd->solution.resources[papd->solution.lengthArrays-1].name,
//			  papd->.aproblem.resources[sp.alternative.indexResource].name);


	  //**************
//	  delete_alternatives(&alternatives);
	  return res;
  }

  int combine_solutions(AproblemPD appd, PSolution ps, PSpPD sp )
  {
	  int res=0;
	  ps->acum+=sp->value;
	  ps->lengthArrays+=1;
	  strcpy( ps->resources[ ps->lengthArrays-1].name,appd.aproblem.resources[sp->alternative.indexResource].name);
	  return res;

  }
  int get_num_subproblems()
  {
	  return 1;
  }
  int get_subproblem(const PAproblemPD  father, PAproblemPD new,Alternative a, int numSubproblems)
  {
	  int res=0;
	  double value;
	  printf("inside get_subproblem");
	  new->aproblem=father->aproblem;

	  //update
	  value=father->aproblem.values[(father->index)*(father->aproblem.numResources)+a.indexResource];
	  new->index=father->index+1;
	  new->solution=father->solution;
	  update_solution(&(new->solution), &a, value,father->aproblem);
	  return res;
  }

  double get_estimate(PAproblemPD appd)//the minimum value my problem will get
  {
	  double res;
	  //if not pruning and in this problem
	  res=GREAT;
	  //if not//TODO
	  return res;
  }

  int get_best(PAproblemPD appd)
  {
	  int res;
	  //if not pruning and in this problem
	  res=SMALL;
	  //if not//TODO
	  return res;
  }
  int get_target(PAproblemPD appd)
  {
	  int res=0;
	  //TODO
	  return res;
  }
  int get_size(const PAproblemPD papd){
	  return papd->aproblem.numTask;
  }
///////////////AUX
  int update_solution(PSolution ps, PAlternative pa, double value, Aproblem ap){
  	int res=0;
  	ps->lengthArrays=ps->lengthArrays+1;
  	ps->acum=(ps->acum)+value;
  	Resource resource;
  	init_resource(&resource,ap.resources[pa->indexResource].name);
  	int i=(ps->lengthArrays)-1;
  	ps->resources[i]=resource;
  	return res;
  }

  int delete_problem_PD(PAproblemPD papd)//let memory free
  {
	  int res=-1;
	  int res1=delete_aproblem(&(papd->aproblem));
	  int res2=delete_solution(&(papd->solution));
	  res= res1+res2;
	  return res;
  }
  int copy_aproblem_PD( PAproblemPD rcv,AproblemPD origin)
  {
	  rcv->aproblem=origin.aproblem;
	  rcv->index=origin.index;
	  Solution sol;
	  init_solution(&sol,rcv->aproblem.numTask);
	  rcv->solution=sol;
	  rcv->solution.acum=origin.solution.acum;
	  rcv->solution.lengthArrays=origin.solution.lengthArrays;
	  for(int i=0;i<rcv->solution.lengthArrays;i++){
		  rcv->solution.resources[i]=origin.solution.resources[i];

	  }
	  return 0;
  }
