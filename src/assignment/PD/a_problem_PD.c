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
	  if(print_all)
	  {
		  printf("\na_problem_PD.c		get_alternatives()		 len=%d in:\n",papd->solution.lengthArrays);

		  for(int j=0;j<papd->solution.lengthArrays;j++ )
		  {
			  printf("%s ", papd->solution.resources[j].name);
		  }
		  printf("\n");
	  }
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
	  int index=papd->index;//where we begin
	  if(print_all)
	  {
		  printf("\na_problem_PD		select_alternative()		Index: %d\n", index);
	  }

	  int num_resources=papd->aproblem.numResources;
	  for(int i=0;i<numAlternatives;i++){

		  actualValue=papd->aproblem.values[index+as[i].indexResource*num_resources];
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
	  if(print_all)
	  {
		  printf("\na_problem_PD		select_alternative()			value and select: %f, %d",selectedValue, selected);
	  }
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
	  if(print_all)
	  {
		  printf("\na_problem_PD		get_solution_base_case()		selected value: %f", selectedValue);
	  }
	  psp->alternative.indexResource=alternative;
	  psp->value=selectedValue;
	  //**************

	  papd->solution.acum+=psp->value;
	  papd->solution.lengthArrays++;
	  Resource resource;
	  init_resource(&resource,papd->aproblem.resources[psp->alternative.indexResource].name,psp->alternative.indexResource);
	  papd->solution.resources[papd->index]=resource;
//	  strcpy(papd->solution.resources[papd->solution.lengthArrays-1].name,
//			  papd->.aproblem.resources[sp.alternative.indexResource].name);

	  //**************
//	  delete_alternatives(&alternatives);
	  if(print_all)
	  {
		  printf("\na_problem_PD		get_solution_base_case()		in psolution: %f", psp->value);
		  printf("\na_problem_PD		get_solution_base_case()		in papd: %f", papd->solution.acum);
		  show_aproblem_PD(papd);
	  }
	  Alternative a=psp->alternative;
//	  update_solution(papd, &a, selectedValue, papd->aproblem);
//	  printf("\n Inside get solution case base without update solution");

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
	  new->aproblem=father->aproblem;

	  //update
	  if(print_all)
	  {
		  printf("\na_problem_PD.c		get_subproblem()		father index: %d", father->index);
		  printf("\na_problem_PD.c		get_subproblem()		num problems= %d", father->aproblem.numResources);
		  printf("\na_problem_PD.c		get_subproblem()		index resource of alt= %d", a.indexResource);
	  }

	  value=father->aproblem.values[(father->index)+(father->aproblem.numResources)*a.indexResource];
	  if(print_all)
	  {
		  printf("\na_problem_PD.c		get_subproblem()		VALUE of ^: %f", value);
	  }

	  new->index=father->index+1;
	  //copy solution
	  new->solution=father->solution;
	  update_solution(new, &a, value,father->aproblem);
	  if(print_all)
	  {
		  printf("\na_problem_PD.c		get_subproblem()		At the end, new solution acum=%f",new->solution.acum);
		  show_solution(&(new->solution));
	  }

	  return res;
  }

  double get_estimate(PAproblemPD appd)//the minimum value my problem will get
  {
	  double res;
	  //dummy prune
	  double worst_value;
	  int num_to_end;
	  //if not pruning
	  if(appd->aproblem.type==MAX)
	  {
		  res=GREAT;
	  }
	  else
	  {
		  res=SMALL;
	  }
	  if(1)//if pruning//TODO and optimizer worst_value in var
	  {
		  if(print_all)
		  {
			  printf("\na_problem_PD.c		get_estimate()		if pruning\n");
		  }
		  num_to_end =appd->aproblem.numTask-appd->solution.lengthArrays;
		  res=appd->solution.acum;
		  if(appd->aproblem.type==MAX)
		  {
			  worst_value=appd->aproblem.values[0];

			    for ( int c = 1 ; c < appd->aproblem.numResources*appd->aproblem.numTask ; c++ )
			    {
			        if ( appd->aproblem.values[c] < worst_value )
			        {
			        	worst_value = appd->aproblem.values[c];
			         }
			    }
		  }
		  else
		  {
			  worst_value=appd->aproblem.values[0];

			    for ( int c = 1 ; c < appd->aproblem.numResources*appd->aproblem.numTask ; c++ )
			    {
			        if ( appd->aproblem.values[c] > worst_value )
			        {
			        	worst_value = appd->aproblem.values[c];
			         }
			    }
		  }

		  res=res+num_to_end*worst_value;
		  if(print_all)
		  {
			  printf("\na_problem_PD.c		get_estimate()		worst value->%f\n",worst_value);
			  printf("\na_problem_PD.c		get_estimate()		estimated->%f\n",res);
		  }
	  }

	  return res;
  }

  double get_target(PAproblemPD appd)//estimated
  {
	  double res;
	  //no prune
//	  if(appd->aproblem.type==MAX)
//	  {
//		  res=SMALL;
//	  }
//	  else
//	  {
//		  res=GREAT;
//	  }
	  //prune
	  res=appd->solution.acum;

	  return res;
  }
  int get_size(const PAproblemPD papd){
	  return papd->aproblem.numTask;
  }
///////////////AUX
  int update_solution(PAproblemPD new, const PAlternative pa, double value, Aproblem ap){
  	int res=0;
//  	ps->lengthArrays=ps->lengthArrays+1;
//  	printf("\n           0000000000000 int value update solution; %f",value);
//  	printf("\n           0000000000000 Inside update sol acum old; %f",ps->acum);
//  	ps->acum=(ps->acum)+value;
//  	printf("\n           0000000000000 Inside update sol acum new; %f",ps->acum);

	  Solution sol;
	  Resource resource;
	  int index_resource=pa->indexResource;

	  init_solution(&sol,ap.numTask);
	  sol.acum=new->solution.acum+value;
	  sol.lengthArrays=(new->solution.lengthArrays)+1;
	  for(int i=0;i<sol.lengthArrays-1;i++)
	  {
		  sol.resources[i]=new->solution.resources[i];
	  }

	  Cadena s;
	  strcpy(s,ap.resources[index_resource].name);
	  init_resource(&resource,s,index_resource);

	  sol.resources[sol.lengthArrays-1]=resource;
	  new->solution=sol;
	 // ps*=sol;
	  //delete_solution(&sol);

//  	init_resource(&resource,ap.resources[index_resource].name,index_resource);
//  	int i=(ps->lengthArrays)-1;
//  	//copy resource
//  	//ps->resources[i]=resource;
//  	ps->resources[i].position=resource.position;
//  	strcpy(ps->resources[i].name, resource.name);
  	return res;
  }

  int delete_problem_PD(PAproblemPD papd)//let memory free
  {
	  int res=-1;
//	  int res1=delete_aproblem(&(papd->aproblem));
//	  int res2=delete_solution(&(papd->solution));
//	  res= res1+res2;
	  return 0;
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
	  if(print_all)
	  {
		  for(int i=0;i<rcv->solution.lengthArrays;i++){
			  rcv->solution.resources[i]=origin.solution.resources[i];
			  printf("\na_problem_PD		copy_aproblem_PD ORIGIN resource %d %s",origin.solution.resources[i].position, origin.solution.resources[i].name);
			  printf("\na_problem_PD		copy_aproblem_PD OUTPUT resource %d %s",rcv->solution.resources[i].position, rcv->solution.resources[i].name);

		  }
	  }
	  return 0;
  }
  int show_aproblem_PD(PAproblemPD papd)
  {
	  show_aproblem(&(papd->aproblem));
	  printf("\nSolution:");
	  printf("\n Len: %d",papd->solution.lengthArrays);
	  printf("\n Acum: %f",papd->solution.acum);
	  for(int i=0;i<papd->solution.lengthArrays;i++)
	  {
		  printf("\n Task %d: Resource %d: %s",i,papd->solution.resources[i].position,papd->solution.resources[i].name);
	  }
	  return 0;
  }
