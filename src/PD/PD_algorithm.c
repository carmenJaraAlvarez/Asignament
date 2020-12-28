/*
 * PD_algorithm.c
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */
#include "PD_algorithm.h"

  Logico is_min(PalgorithmPD palg){
	  Logico res=FALSE;
	  if(palg->ppd.aproblem.type==MIN){
		  res=TRUE;
	  }
	  return res;
  }
  Logico is_max(PalgorithmPD palg){
	  Logico res=FALSE;
	  if(palg->ppd.aproblem.type==MAX){
		  res=TRUE;
	  }
	  return res;
  }
  int init_algorithmPD(PalgorithmPD palg, ArrayAproblemPD problems){
	  int res=0;
	  palg->isRandomize=FALSE;//we are not using
	  palg->sizeRef=100;//we are not using
	  if(is_min(palg)){
		  palg->best=999999.;//TODO
	  }
	  else{
		  palg->best=-999999.;//TODO
	  }

	  return res;
  }
  int exec_algorithm(PalgorithmPD palg){
	  int res=0;
	  Solution sol;
	  SpPD sp;
	  do{
		  for(int i=0;i<palg->num_problemas;i++){
			  pD(palg,&sp);
		  }
	  }while(palg->isRandomize && get_PDsolution(palg,&sol)!=0);
	  return res;
  }
  int randomize(PalgorithmPD palg,ArrayAlternatives as){
	  int res=0;
	  if(palg->isRandomize && size(palg->ppd)>palg->sizeRef){
		  //TODO change as to new smaller random array of alternatives
	  }
	  return res;
  }
  int update_best(PalgorithmPD palg){
	  int res=0;
	  double target=get_target(palg->ppd);
	  if(is_min(palg) && target<palg->best){
		  palg->best=get_target(palg->ppd);
	  }
	  else if(is_max(palg) && target>palg->best){
		  palg->best=get_target(palg->ppd);
	  }
	  return res;
  }
  int get_PDsolution(PalgorithmPD palg, PSolution psol){

	  int res=0;
	  return res;;
  }
  int pD(PalgorithmPD palg, PSpPD sp){
	  int res=0;
	  ArraySpPD sps;
	  //*****partial solution of an alternative
	  //if array of Partial solution contains>>return
	  if(FALSE){//TODO

	  }//else is (case base)
	  else if(is_base_case(&(palg->ppd))){
		  Solution sol;
		  SpPD sp;
		  get_solution_base_case(&(palg->ppd),&sp);
  	  	  //array of partial solution add p.getSolutionCaseBAse
  	  	  // p.getSolutionCaseBAse
	  }
	  else{	//else
		 //get alternatives
		  ArrayAlternatives as;
		  int numAlternatives=get_alternatives(&(palg->ppd), as);
		  randomize(palg,as);
		 //for every alternative
		  for(int i=0;i<numAlternatives;i++){
			  //if not prune
			  if((is_min(palg) && get_estimate(palg->ppd)>=palg->best)
					  || (is_max(palg) && get_estimate(palg->ppd)<=palg->best)){
				  int numSubproblems=get_num_subproblems();
				  Logico existsSolution=TRUE;
				  for(int j=0;j<numSubproblems;j++){
					  AproblemPD appdNew;
					  get_subproblem(&palg->ppd, &appdNew, as[i],numSubproblems);
					  palg->ppd=appdNew;
					  pD(palg, sp);
					  if(!sp){//TODO we need a var for no existing solution
						  existsSolution=FALSE;
						  break;
					  }
				  //sp subproblem to array //here not used
				  ArraySpPD arraySp={sp};
				  }
			  if (existsSolution){
				  //combine_solutions();//TODO
				  //add to alternatives array
				  //select the best
			  }
			  //filter alternatives
			  //if exists alternative in array>>select
			  //sp is ready to return
			  //array of partial solution add
			  }
		  }
		  update_best(palg);
	  }
	  return res;
  }
