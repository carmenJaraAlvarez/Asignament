/*
 * PD_algorithm.c
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */
#include "PD_algorithm.h"



  Logico is_min(const PalgorithmPD palg)
  {
	  Logico res=FALSE;
	  if(palg->ppd.aproblem.type==MIN)
	  {
		  res=TRUE;
	  }
	  return res;
  }
  Logico is_max(const PalgorithmPD palg)
  {
	  Logico res=FALSE;
	  if(palg->ppd.aproblem.type==MAX)
	  {
		  res=TRUE;
	  }
	  return res;
  }
  int init_algorithmPD(PalgorithmPD palg,AproblemPD ap)
  {
	  int res=0;
	  palg->isRandomize=FALSE;//we are not using
	  palg->sizeRef=100;//we are not using
	  if(is_min(palg))
	  {
		  palg->best=999999.;//TODO
	  }
	  else{
		  palg->best=-999999.;//TODO
	  }
	  int num=get_max_num_problems(&(ap));
	  palg->problems=(AproblemPD*)malloc(sizeof(AproblemPD)*num);

	  palg->ppd=ap;
	  copy_aproblem_PD(&(palg->problems[0]),ap);

	  palg->num_problems=1;//TODO anothers alternatives

	  //palg->problems[0]=palg->ppd;
	  palg->num_solved=0;

	  palg->solvedProblems=(AproblemPD*)malloc(sizeof(AproblemPD)*num);
	  return res;
  }

  int exec_algorithm(PalgorithmPD palg)
  {
	  int res=0;
	  Solution sol;
	  do
	  {
		  for(int i=0;i<get_num_subproblems();i++)
		  {
			  pD(palg);
		  }
	  }while(palg->isRandomize && get_PDsolution(palg,&sol)!=0);
	  return res;
  }

  int randomize(PalgorithmPD palg,PAlternative as)
  {
	  int res=0;
	  if(palg->isRandomize && get_size(&(palg->ppd))>palg->sizeRef)
	  {
		  //TODO change as to new smaller random array of alternatives
	  }
	  return res;
  }

  int update_best(PalgorithmPD palg,const PAproblemPD papd)
  {
	  int res=0;
	  double target=get_target(papd);
	  if(is_min(palg) && target<palg->best)
	  {
		  palg->best=target;
		  send_best(palg);//TODO if severals
	  }
	  else if(is_max(palg) && target>palg->best)
	  {
		  palg->best=target;
		  send_best(palg);
	  }
	  return res;
  }

  int get_PDsolution(PalgorithmPD palg, PSolution psol)
  {
	  int res=0;

	  if(palg->num_solved==0){
		  printf("there is not solution");
	  }
	  else if(palg->num_solved==1){


		  palg->ppd=palg->solvedProblems[0];//TODO needed??
		  palg->best=palg->solvedProblems[0].solution.acum;
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

  int pD(PalgorithmPD palg)
  {
	  int res=0;
	  AproblemPD appd=palg->ppd;
	  int len_problems=palg->num_problems;
	  int lengthNewArrayAppd=palg->num_problems;
	  int index;
	  index=palg->ppd.index;
	  printf("\n PPPPPPPPPPPPPPPPPPPPPPP index of ppd in pd: %d", index);
	  printf("\n PPPPPPPPPPPPPPPPPPPPPPP num problems: %d", len_problems);
	  for(int m=0;m<len_problems;m++){
		  show_aproblem_PD(&(palg->problems[m]));
	  }
	  for(int m=0;m<lengthNewArrayAppd;m++){
		  AproblemPD newArrayAppd[get_max_num_problems(&appd)+lengthNewArrayAppd];
		  printf("\n***************************************\n");
		  printf("\nfirst len array problem in PD = %d", lengthNewArrayAppd);
		  //get previous problems
		  int numPreviousProblems=getPreviousProblems(palg, newArrayAppd);

		  printf("\n PREVIOUS PROBLEM_________________________%d",m);
		  show_aproblem_PD(&(palg->problems[m]));
		  printf("\n COPY PROBLEM_________________________%d",m);
		  show_aproblem_PD(&(newArrayAppd[m]));

		  int max_num_alternatives=get_max_num_alternatives(&appd);
		  Alternative as[max_num_alternatives];
		  printf("\n 	000000000000 num max alternatives %d",max_num_alternatives);
		  init_alternative_array(&as,max_num_alternatives);
		  int numAlternatives=get_alternatives(&(newArrayAppd[m]), as);

		  if(numAlternatives==0)
		  {
				  //TODO
				  printf("\n no alternatives\n");
		  }
		  else
		  {

			  if(is_base_case(&newArrayAppd[m]))
			  {
					  printf("\nfirst get solution case base");
					  show_aproblem_PD(&newArrayAppd[m]);
					  printf("\n````````````````````````````\n");
					  //show_aproblem_PD(&(palg->problems[39]));//TODO check
					  SpPD sp;
					  get_solution_base_case((&newArrayAppd[m]),&sp);
					  printf("\nPPPPPPPPPPPPPPPPPPPPPPPP case base");
					  printf("\n in base case. best: %f",palg->best);
					  printf("\n in base case. acum: %f",newArrayAppd[m].solution.acum);
					  AproblemPD solved[100];//TODO
					  //palg->solvedProblems=&solved;
					  if(newArrayAppd[m].solution.acum==palg->best)//more than one solution
					  {
						  copy_aproblem_PD( &(palg->solvedProblems[palg->num_solved]),newArrayAppd[m]);
						  palg->num_solved++;
						  update_best(palg,&(newArrayAppd[m]));
					  }
					  else if(newArrayAppd[m].solution.acum>palg->best)//now this is the only one best solution
					  {
						  palg->solvedProblems=&solved;
						  printf("\nThis solution is the best now. acum: %f",newArrayAppd[m].solution.acum);
						  palg->num_solved=1;
						  show_aproblem_PD(&(newArrayAppd[m]));
						  update_best(palg,&(newArrayAppd[m]) );
						  printf("\npost update best======================\n");
						  //show_aproblem_PD(&(palg->problems[39]));//TODO check
						  printf("\n NUM SOLVED: %d",palg->num_solved);
						  printf("\n UPDATED BEST TO %f",palg->best);
						  printf("\n we are going to copy the problem in solved");
						  //AproblemPD solved;
						  copy_aproblem_PD( &solved,newArrayAppd[m]);
						  printf("\npd problem aux======================\n");
						  show_aproblem_PD(&solved);

						  //printf("\npost copy solved 39======================\n");
						 // show_aproblem_PD(&(palg->problems[39]));//TODO check
						  printf("\npost copy solved 0======================\n");
						  show_aproblem_PD(&(palg->solvedProblems[0]));

					  }
					  printf("     is base case and takes alternative: %d\n", sp.alternative.indexResource);

			  }
			  else//not case base
			  {
				  //get new problems
				  printf("        Alternatives: ");
				  for(int k=0;k<numAlternatives;k++)
				  {
					  printf("%d ", as[k].indexResource);
				  }
				  printf("\n");
				  randomize(palg,as);//now not using
				  Logico ismin;
				  Logico ismax;
				  for(int u=0;u<numAlternatives;u++)
				  {
					  //prune
					  ismin=is_min(palg);
					  ismax=is_max(palg);

					  double estimated=get_estimate(&newArrayAppd[m]);
					  if((ismin && estimated<=final_alg.best)
										  || (ismax && estimated>=final_alg.best))
					  {
						  printf("\n ESTIMATED IN PD %f\n FINAL ALG BEST %f", estimated, final_alg.best);
						  final_alg.best=estimated;//TODO mpi var
						  int numSubproblems=get_num_subproblems();
						  AproblemPD appdNew;
						  for(int j=0;j<numSubproblems;j++)
						  {
							  initAProblemPD(&appdNew,&(palg->ppd.aproblem));

							  get_subproblem(&newArrayAppd[m], &appdNew, as[u],numSubproblems);
							  printf("\ninside pd acum post get subproblem father: %f",palg->ppd.solution.acum);
							  printf("\ninside pd acum post get subproblem new m: %f",appdNew.solution.acum);
							  printf("\n     is NOT base case: last appdNew sol: %s\n",appdNew.solution.resources[appdNew.solution.lengthArrays-1].name);
							  printf("     i=%d of %d alternatives\n",u, numAlternatives);
							  //if problem//TODO
									  //add problem to new array
							  copy_aproblem_PD( &(newArrayAppd[lengthNewArrayAppd]),appdNew);
							 lengthNewArrayAppd++;
							  printf("\n***************************************\n");
							  printf("\nadding alt len array problem in PD = %d", lengthNewArrayAppd);

						  }//end for num subproblem=1
					  }//end if not prune
					  else
					  {
						  MPE_Log_event(event6, 0, "prune");
						  if(estimated>final_alg.best )//TODO max min
						  {
							  final_alg.best=estimated;
						  }
					  }
				  }//end for alternatives
				  for(int w=len_problems; w<lengthNewArrayAppd;w++)//TODO optimizer
				  {
					  //palg->problems[w]=newArrayAppd[problems_index];

					  printf("\n PPPPPPPPPPPPPPPPPPPP inside PD, new problem %d to copy", w);
					  show_aproblem_PD(&(newArrayAppd[w]));

					  //copy_aproblem_PD( &(palg->problems[0]),newArrayAppd[problems_index]);
					  palg->problems[w]=newArrayAppd[w];
					  printf("\n PPPPPPPPPPPPPPPPPPPP inside PD, new problem %d copied in alg", w);
					  show_aproblem_PD(&(palg->problems[w]));
					  //copy_aproblem_PD( &(newArrayAppd[lengthNewArrayAppd]),appdNew);
				  }
				  palg->num_problems=lengthNewArrayAppd;
			  }//end else (not base case)
//					}//end else (exits alternative)
//			  }//end for every previous problem

			  //change problems array and add num problems to array


		  }//end if num previous>0
	  }//end for size
	  return res;
  }


  int getPreviousProblems(PalgorithmPD palg,PAproblemPD problems){

	  for(int i=0;i<palg->num_problems;i++)
	  {
		  problems[i]=palg->problems[i];
	  }
	  return palg->num_problems;

  }

  int delete_algorithmPD(PalgorithmPD palg)
  {
//	  free(palg->solvedProblems);
//	  free(palg->problems);
//	  delete_problem_PD(&(palg->ppd));
	  return 0;
  }


  double get_best(PalgorithmPD palg)
  {
	  double res;
	  double global;
	  //if not pruning
	  if(palg->ppd.aproblem.type==MAX){
		  res=SMALL;
	  }
	  else
	  {
		  res=GREAT;
	  }

	  if(1)//prune //TODO
	  {
		  if(numprocs>2)//more than 1 slave
		  {
			  waitting_best(palg);
		  }
		  res=palg->best;
	  }

	  return res;
  }
