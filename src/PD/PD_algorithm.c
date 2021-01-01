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
  int init_algorithmPD(PalgorithmPD palg, ArrayAproblemPD problems,AproblemPD ap){
	  int res=0;
	  palg->isRandomize=FALSE;//we are not using
	  palg->sizeRef=100;//we are not using
	  if(is_min(palg)){
		  palg->best=999999.;//TODO
	  }
	  else{
		  palg->best=-999999.;//TODO
	  }
	  palg->ppd=ap;
	  //palg->problems=problems;
	  for(int i=0;i<palg->num_problemas;i++){
		  palg->problems[i]=problems[i];
	  }
	  palg->num_problemas=1;
	  palg->num_solved=0;
	  return res;
  }
  int exec_algorithm(PalgorithmPD palg){
	  int res=0;
	  Solution sol;
	  SpPD sp;
	  do{
		  for(int i=0;i<palg->num_problemas;i++){
			  pD(palg,palg->ppd,&sp);
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
	  if(palg->num_solved==0){
		  printf("there is not solution");
	  }
	  else if(palg->num_solved==1){
		  palg->ppd=palg->solvedProblems[0];
		  printf("there is only one solution");
	  }else{
		  for(int i=0;i<palg->num_solved;i++){
			  	  if(palg->solvedProblems[i].solution.acum>=palg->best){
			  		palg->ppd=palg->solvedProblems[i];
			  		palg->best=palg->solvedProblems[i].solution.acum;//TODO
			  	  }
			  }
		  printf("Selected one of the best solutions");//TODO
	  }

	  return res;;
  }
  int count=0;
  int pD(PalgorithmPD palg, AproblemPD appd,PSpPD sp){
	  printf("\n\n*******************In pD: Round %d\n",count);
	  count++;
	  int res=0;
	  ArrayAlternatives as;
	  int numAlternatives=get_alternatives(&appd, as);
	  if(numAlternatives==0){
		  return res;
		  printf("\n no alternatives\n");
	  }else{
	  if(is_base_case(&appd)){
			  SpPD sp;
			  get_solution_base_case(&appd,&sp);
			  appd.solution.acum+=sp.value;
			  appd.solution.lengthArrays++;
			  strcpy(appd.solution.resources[appd.solution.lengthArrays-1].name,
					  appd.aproblem.resources[sp.alternative.indexResource].name);
			  palg->solvedProblems[palg->num_solved]=appd;
			  palg->num_solved++;
			  printf("     is base case with solution: %s\n",appd.solution.resources[appd.solution.lengthArrays-1].name);
		  }
	  else{

		  printf("        Alternatives: ");
		  for(int k=0;k<numAlternatives;k++){
			  printf("%d ", as[k].indexResource);
		  }
		  printf("\n");
		  randomize(palg,as);//not using
		  Logico ismin;
		  Logico ismax;
		  Solution sol;
		 //for every alternative
		  for(int i=0;i<numAlternatives;i++){
			  //if not prune
			  ismin=is_min(palg);
			  ismax=is_max(palg);
			  double estimated=get_estimate(appd);
			  ArraySpPD arraySp;
			  if((ismin && estimated<=palg->best)
					  || (ismax && estimated>=palg->best)){
				  int numSubproblems=get_num_subproblems();
				  Logico existsSolution=TRUE;
				  for(int j=0;j<numSubproblems;j++){
					  AproblemPD appdNew;

					  get_subproblem(&appd, &appdNew, as[i],numSubproblems);
					  printf("     is NOT base case: last appdNew sol: %s\n",appdNew.solution.resources[appd.solution.lengthArrays-1].name);
					  printf("     i=%d of %d alternatives",i, numAlternatives);
					  //palg->ppd=appdNew;
					  pD(palg,appdNew, sp);
				  }
			  }

		  }
	  }
	  }
	  return res;
  }
