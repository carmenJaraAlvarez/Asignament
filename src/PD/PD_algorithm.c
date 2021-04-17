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
		  palg->best=GREAT;
	  }
	  else{
		  palg->best=SMALL;
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

  int exec_algorithm(PalgorithmPD palg,double * buffer,MPI_Request * request_b,int * buffer_w, MPI_Request * request_w )
  {
	  int res=0;
	  Solution sol;
	  do
	  {
		  for(int i=0;i<get_num_subproblems();i++)
		  {
			  pD(palg,buffer, request_b, buffer_w, request_w);
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

  int update_best(PalgorithmPD palg,const PAproblemPD papd)//Function for case base
  {
	  int res=0;
	  double target=get_target(papd);//acum
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
		  if(print_all)
		  {
			  printf("\nPD_algorithm.c	get_PDsolution		there is only one solution");
		  }

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

  int pD(PalgorithmPD palg,double * buffer,MPI_Request * request_b,int * buffer_w,MPI_Request * request_w)
  {
	  init_transfered(&transfered);
	  int res=0;
	  AproblemPD appd=palg->ppd;
	  int len_problems=palg->num_problems;
	  int lengthNewArrayAppd=palg->num_problems;
	  int index;
	  index=palg->ppd.index;
	  if(print_all)
	  {
		  printf("\nPD_algorithm.c		pD		best in palg: %f", palg->best);
		  printf("\nPD_algorithm.c		pD		best in final_alg: %f", final_alg.best);
		  printf("\nPD_algorithm.c		pD		index of ppd in pd: %d", index);
		  printf("\nPD_algorithm.c		pD		num problems: %d", len_problems);
		  for(int m=0;m<len_problems;m++){
			  show_aproblem_PD(&(palg->problems[m]));
		  }
	  }
	  for(int m=0;m<lengthNewArrayAppd;m++){
		  ///redistribution
		  int up=0;
		  if((lengthNewArrayAppd-m)>1){//to not send the only one
			  waiting_confirming(&transfered);
			  int rcvr=0;
			  up = waiting_petition(buffer_w,request_w,palg,m,&rcvr);
			  if(up)
			  {
				  transfered.transfered[transfered.len_transfered]=m;
				  transfered.receivers[transfered.len_transfered]=rcvr;
				  transfered.len_transfered++;
			  }
		  }
		  m=m+up;
		  ///////////
		  waiting_best(buffer, request_b);
		 // double test=get_best(palg);
		  if((palg->best<final_alg.best && palg->ppd.aproblem.type==MAX) ||
				  (palg->best>final_alg.best && palg->ppd.aproblem.type==MIN ) ){
			  palg->best=final_alg.best;
			  printf("\nPD_algorithm.c		pD()		-----------new best IN %f",palg->best);
		  }

		  int max_num_problem=get_max_num_problems(&appd);
		  int len=max_num_problem+lengthNewArrayAppd;
		  newArrayAppd = (AproblemPD*)malloc(len * sizeof(AproblemPD));

		  if(print_all)
		  {
			  printf("\nPD_algorithm.c		pD		first len array problem in PD = %d", lengthNewArrayAppd);

		  }
		  //get previous problems
		  int numPreviousProblems=getPreviousProblems(palg, newArrayAppd);
		  if(print_all)
		  {
			  printf("\nPD_algorithm.c		pD		PREVIOUS PROBLEM %d",m);
			  show_aproblem_PD(&(palg->problems[m]));
			  printf("\nPD_algorithm.c		pD		COPY PROBLEM %d",m);
			  show_aproblem_PD(&(newArrayAppd[m]));
		  }

		  int max_num_alternatives=get_max_num_alternatives(&appd);
		  Alternative as[max_num_alternatives];
		  if(print_all)
		  {
			  printf("\nPD_algorithm.c		pD		num max alternatives %d",max_num_alternatives);
		  }

		  init_alternative_array(&as,max_num_alternatives);
		  int numAlternatives=get_alternatives(&(newArrayAppd[m]), as);

		  if(numAlternatives==0)
		  {
				  //TODO
				  printf("\nPD_algorithm.c		pD		no alternatives\n");
		  }
		  else
		  {
			  //if(is_base_case(&newArrayAppd[m]))
			  if(is_base_case(&(palg->problems[m])))
			  {
				  if(print_all)
				  {
					  printf("\nPD_algorithm.c		pD		before get solution case base");
					 // show_aproblem_PD(&newArrayAppd[m]);
					  printf("\nPD_algorithm.c		pD		before get solution case base.best in palg: %f", palg->best);
					  printf("\nPD_algorithm.c		pD		before get solution case base.best in final_alg: %f", final_alg.best);
				  }
				  //show_aproblem_PD(&(palg->problems[39]));//TODO check
				  SpPD sp;
				  get_solution_base_case((&(palg->problems[m])),&sp);
				  if(print_all)
				  {
					  printf("\nPD_algorithm.c		pD		case base");
					  printf("\nPD_algorithm.c		pD		post get solution case base. best palg: %f",palg->best);
					  printf("\nPD_algorithm.c		pD		post get solution case base. best final: %f",final_alg.best);
					  printf("\nPD_algorithm.c		pD		in base case. acum: %f",newArrayAppd[m].solution.acum);
				  }
				  //AproblemPD solved[100];//TODO
				  //palg->solvedProblems=&solved;

				  if(palg->num_solved>0 && palg->problems[m].solution.acum==palg->solvedProblems[0].solution.acum)//more than one solution
				  {
					  copy_aproblem_PD( &(palg->solvedProblems[palg->num_solved]),palg->problems[m]);
					  palg->num_solved++;
					  //update_best(palg,&(newArrayAppd[m]));
				  }
				  else if(palg->num_solved==0 ||
						  (palg->num_solved>0 && palg->problems[m].solution.acum>palg->solvedProblems[0].solution.acum))//now this is the only one best solution
				  {
					  copy_aproblem_PD( &(palg->solvedProblems[0]),palg->problems[m]);
					  palg->num_solved=1;
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		This solution is the best now. acum: %f",newArrayAppd[m].solution.acum);
						  show_aproblem_PD(&(newArrayAppd[m]));
						  printf("\nPD_algorithm.c		pD		pre update best palg: %f",palg->best);
						  printf("\nPD_algorithm.c		pD		pre update best final_alg: %f", final_alg.best);
					  }
					  update_best(palg,&(palg->solvedProblems[m]) );
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		post update best");
						  //show_aproblem_PD(&(palg->problems[39]));//TODO check
						  printf("\nPD_algorithm.c		pD		NUM SOLVED: %d",palg->num_solved);
						  printf("\nPD_algorithm.c		pD		post update best palg: %f",palg->best);
						  printf("\nPD_algorithm.c		pD		post update best final_alg: %f", final_alg.best);
						  printf("\nPD_algorithm.c		pD		we are going to copy the problem in solved");
					  }
					  //AproblemPD solved;
					  //copy_aproblem_PD( &solved,newArrayAppd[m]);
					  if(print_all)
					  {
						 // printf("\nPD_algorithm.c		pD		problem aux\n");
						  //show_aproblem_PD(&solved);
						  //printf("\npost copy solved 39======================\n");
						  //show_aproblem_PD(&(palg->problems[39]));//TODO check
						  printf("\nPD_algorithm.c		pD		post copy solved 0\n");
						  show_aproblem_PD(&(palg->solvedProblems[0]));
					  }
				  }

				  if(print_all)
				  {
					  printf("\nPD_algorithm.c		pD		is base case and takes alternative: %d\n", sp.alternative.indexResource);
				  }
			  }
			  else//not case base
			  {
				  //get new problems
				  if(print_all)
				  {
					  printf("\nPD_algorithm.c		pD		Not case base. Alternatives: ");
//					  for(int k=0;k<numAlternatives;k++)
//					  {
//						  printf("%d ", as[k].indexResource);
//					  }
					  printf("\n");
				  }

				  randomize(palg,as);//now not using
				  Logico ismin;
				  Logico ismax;
				  for(int u=0;u<numAlternatives;u++)
				  {
					  //prune control
					  ismin=is_min(palg);
					  ismax=is_max(palg);

					  double b_estimated=get_best_estimate(&newArrayAppd[m]);
					  if((ismin && b_estimated<=final_alg.best)
										  || (ismax && b_estimated>=final_alg.best))//no prune, go on
					  {
						  if(print_all)
						  {
							  printf("\nPD_algorithm.c		pD		NO PRUNE best in palg: %f", palg->best);
							  printf("\nBEST ESTIMATED IN PD var %f\n .IN FINAL ALG BEST %f", b_estimated, final_alg.best);
						  }

						  //case no prune,control our worst is better than global to change it
						  double w_estimated=get_worst_estimate(&newArrayAppd[m]);
						  if((w_estimated>final_alg.best && ismax) ||
								  (w_estimated<final_alg.best && ismin)  )
						  {
							  if(print_all)
							  {
								  printf("\nPD_algorithm.c		pD		NO PRUNE AND CHANGE final_alg.best: %f to worst estimated %f", palg->best, w_estimated);

							  }

							  final_alg.best=w_estimated;
							  send_best(&final_alg);
						  }
						  if((w_estimated>palg->best && ismax) ||
								  (w_estimated<palg->best && ismin)  )
						  {
							  if(print_all)
							  {
								  printf("\nPD_algorithm.c		pD		NO PRUNE AND CHANGE palg->best: %f to worst estimated %f", palg->best, w_estimated);

							  }
							  palg->best=w_estimated;
						  }
						  ////////////////////////////////////////////////////////////////////
						  int numSubproblems=get_num_subproblems();
						  AproblemPD appdNew;
						  for(int j=0;j<numSubproblems;j++)
						  {
							  initAProblemPD(&appdNew,&(palg->ppd.aproblem));

							  get_subproblem(&newArrayAppd[m], &appdNew, as[u],numSubproblems);
							  if(print_all)
							  {
								  printf("\nPD_algorithm.c		pD		acum post get subproblem father: %f",palg->ppd.solution.acum);
								  printf("\nPD_algorithm.c		pD		acum post get subproblem new m: %f",appdNew.solution.acum);
								  printf("\nPD_algorithm.c		pD		is NOT base case: last appdNew sol: %s\n",appdNew.solution.resources[appdNew.solution.lengthArrays-1].name);
								  printf("\nPD_algorithm.c		pD		i=%d of %d alternatives\n",u, numAlternatives);
							  }
							  //if problem//TODO
								  //add problem to new array
							  copy_aproblem_PD( &(newArrayAppd[lengthNewArrayAppd]),appdNew);
							  lengthNewArrayAppd++;
							  if(print_all)
							  {
								  printf("\nPD_algorithm.c		pD		adding alt len array problem in PD = %d", lengthNewArrayAppd);
							  }

						  }//end for num subproblem=1
					  }//end if not prune
					  else//prune
					  {
						  MPE_Log_event(event6, 0, "prune in PD");

					  }
				  }//end for alternatives
				  for(int w=len_problems; w<lengthNewArrayAppd;w++)//TODO optimizer
				  {
					  //palg->problems[w]=newArrayAppd[problems_index];
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		new problem %d to copy", w);
						  show_aproblem_PD(&(newArrayAppd[w]));
					  }
					  //copy_aproblem_PD( &(palg->problems[0]),newArrayAppd[problems_index]);
					  palg->problems[w]=newArrayAppd[w];
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		new problem %d copied in alg", w);
						  show_aproblem_PD(&(palg->problems[w]));
					  }
					  //copy_aproblem_PD( &(newArrayAppd[lengthNewArrayAppd]),appdNew);
				  }
				  palg->num_problems=lengthNewArrayAppd;
				  if(print_all)
				  {
					  printf("\nPD_algorithm.c		pD()		end of not case base solution");
				  }

			  }//end else (not base case)
		  }//end if num alt>0
		  free(newArrayAppd);
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

	  if(print_all)//prune //TODO
	  {
		  if(numprocs>2)//more than 1 slave
		  {
			  printf("\n-------------------- init waiting_best");
			  //waiting_best(&requestb);
		  }
		  res=palg->best;
	  }

	  return res;
  }

