/*
 * PD_algorithm.c
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */
#include "PD_algorithm.h"

AproblemPD appd_for_top_deep;

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
	  init_transfered(&transfered);
	  init_tuple_prune(&tuple_prune_data);
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
	  int num;
	  if(deep)//deep
	  {
		  num=get_max_num_problems_deep(&(ap));
	  }
	  else
	  {
		  num=get_max_num_problems(&(ap));
	  }
	  if(print_all)
	  {
		  printf("\nPD_algorithm.c **%d->%d",deep,num);
	  }


	  palg->problems=(AproblemPD*)malloc(sizeof(AproblemPD)*num*2);

	  palg->ppd=ap;
	  copy_aproblem_PD(&(palg->problems[0]),ap);

	  palg->num_problems=1;//TODO anothers alternatives

	  //palg->problems[0]=palg->ppd;
	  palg->num_solved=0;

	  palg->solvedProblems=(AproblemPD*)malloc(sizeof(AproblemPD)*num);
	  return res;
  }

  int exec_algorithm
  (
		  PalgorithmPD palg,
		  double * buffer,
		  MPI_Request * request_b,
		  int * buffer_w,
		  MPI_Request * request_w,
		  int first_search
  )
  {
	  int id;
	  MPI_Comm_rank(MPI_COMM_WORLD,&id);

	  deep=first_search;
	  if(print_all)
	  {
		  printf("\nPD_algorithm.c	exec_algorithm()	p%d deep:%d",id,deep);
	  }
	  int res=0;
	  Solution sol;
	  do
	  {
		  for(int i=0;i<get_num_subproblems();i++)
		  {
			  pD(palg,buffer, request_b, buffer_w, request_w);
		  }
	  }while(palg->isRandomize && get_PDsolution(palg,&sol)!=0);
	  if(print_all)
	  {
		  printf("\nPD_algorithm.c		exec_algorithm()		p%d outgoing",id);
	  }
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
		  send_best(palg);
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
		  if(print_all)
		  {
			  printf("there is not solution");
		  }
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
		  if(print_all)
		  {
			  printf("Selected one of the best solutions");//TODO to screen
		  }

	  }

	  return res;;
  }
  void update_alg_best(PalgorithmPD palg)
  {
	  if((palg->best<final_alg.best && palg->ppd.aproblem.type==MAX) ||
	  	  					  (palg->best>final_alg.best && palg->ppd.aproblem.type==MIN ) )
	  	  {
	  		  palg->best=final_alg.best;
	  		  if(print_all)
	  		  {
	  			  printf("\nPD_algorithm.c		pD()		-----------new best IN palg%f",palg->best);

	  		  }
	  	  }
	  	  if((palg->best>final_alg.best && palg->ppd.aproblem.type==MAX) ||
	  	  					  (palg->best<final_alg.best && palg->ppd.aproblem.type==MIN ) )
	  	  {
	  		  final_alg.best=palg->best;
	  		  if(print_all)
	  		  {
	  			  printf("\nPD_algorithm.c		pD()		-----------new best IN final alg%f",palg->best);

	  		  }
	  	  }
  }

  int pD(PalgorithmPD palg,double * buffer,MPI_Request * request_b,int * buffer_w,MPI_Request * request_w)
  {
	  int id;
	  MPI_Comm_rank(MPI_COMM_WORLD,&id);

	  if(print_all)
	  {
		  printf("\nPD_algorithm.c	 pD()		*******************p%d init . trasfered:",id);
		  show_transfered(&transfered);
	  }
	  int res=0;
	  int serial=0;
	  if(numprocs==1){
		  if(print_all)
		  {
			  printf("\nPD_algorithm.c	 pD()		*******************SERIAL");
		  }
		  serial=1;
	  }
	  AproblemPD appd=palg->ppd;
	  int len_problems=palg->num_problems;
	  int lengthNewArrayAppd=palg->num_problems;
	  int index;
	  index=palg->ppd.index;
	  update_alg_best(palg);
	  if(print_all)
	  {
		  printf("\nPD_algorithm.c		pD		best in palg: %f", palg->best);
		  printf("\nPD_algorithm.c		pD		best in final_alg: %f", final_alg.best);
		  printf("\nPD_algorithm.c		pD		index of ppd in pd: %d", index);
		  printf("\nPD_algorithm.c		pD		num problems: %d", len_problems);
		  for(int m=0;m<len_problems;m++){
			  show_aproblem_PD(&(palg->problems[m]));
		  }
		  printf("\nPD_algorithm.c		pD		deep: %d", deep);
	  }
	  if(!deep)//BFS
	  {
		  if(print_all)
		  {
			  printf("\n lengthNewArrayAppd %d",lengthNewArrayAppd);
		  }
		  for(int m=0;m<lengthNewArrayAppd;m++){
			  ///redistribution
			  int up=0;
			  if((lengthNewArrayAppd-m)>1 && !serial)
			  {//to not send the only one
				  int rcvr=0;
				  up = waiting_petition(buffer_w,request_w,palg,m,&rcvr);
				  if(up)
				  {
					  add_transfered(&transfered,(&palg->problems[m]),rcvr);
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c	 pD()		add_trasfered rcvr %d",rcvr);
						  show_transfered(&transfered);
					  }
				  }
			  }
			  m=m+up;
			  if(print_all)
			  {
				  printf("\nPD_algorithm.c	 pD()		m:%d",m);
			  }
			  if(!serial)
			  {
				  waiting_confirming(&transfered);
				  waiting_best(buffer, request_b);
			  }

			  update_alg_best(palg);
			  int max_num_problem;
			  if(deep)
			  {
				  max_num_problem=get_max_num_problems_deep(&appd);
			  }
			  else
			  {
				  max_num_problem=get_max_num_problems(&appd);
			  }

			  int len=max_num_problem+lengthNewArrayAppd;

			  if(print_all)
			  {
				  printf("\n**%d->%d",deep,max_num_problem);
				  printf("\nPD_algorithm.c		pD		first len array problem in PD = %d", lengthNewArrayAppd);
				  printf("\nPD_algorithm.c		pD		len=max+lengnew->%d+%d=%d", max_num_problem,lengthNewArrayAppd,len);


			  }

			  int numPreviousProblems=palg->num_problems;

			  int max_num_alternatives=get_max_num_alternatives(&appd);
			  Alternative as[max_num_alternatives];
			  if(print_all)
			  {
				  printf("\nPD_algorithm.c		pD		num max alternatives %d",max_num_alternatives);
			  }

			  init_alternative_array(&as,max_num_alternatives);
			  int numAlternatives=get_alternatives(&(palg->problems[m]), as);

			  if(numAlternatives==0)
			  {
					  //TODO
					  printf("\nPD_algorithm.c		pD		no alternatives\n");
			  }
			  else
			  {

				  if(is_base_case(&(palg->problems[m])))
				  {
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		before get solution case base");
						 // show_aproblem_PD(&newArrayAppd[m]);
						  printf("\nPD_algorithm.c		pD		before get solution case base.best in palg: %f", palg->best);
						  printf("\nPD_algorithm.c		pD		before get solution case base.best in final_alg: %f", final_alg.best);
					  }

					  SpPD sp;
					  get_solution_base_case((&(palg->problems[m])),&sp);
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		case base");
						  printf("\nPD_algorithm.c		pD		post get solution case base. best palg: %f",palg->best);
						  printf("\nPD_algorithm.c		pD		post get solution case base. best final: %f",final_alg.best);
						  printf("\nPD_algorithm.c		pD		in base case. acum: %f",palg->problems[m].solution.acum);
					  }

					  if(palg->num_solved>0 && palg->problems[m].solution.acum==palg->solvedProblems[0].solution.acum)//more than one solution
					  {
						  copy_aproblem_PD( &(palg->solvedProblems[palg->num_solved]),palg->problems[m]);
						  palg->num_solved++;
						  update_best(palg,&(palg->solvedProblems[0]) );
						  if(print_all)
						  {
							  printf("\nPD_algorithm.c		pD		more than one solution");
						  }
					  }
					  else if(palg->num_solved==0 ||
							  (palg->num_solved>0 && palg->problems[m].solution.acum>palg->solvedProblems[0].solution.acum))//now this is the only one best solution
					  {
						  copy_aproblem_PD( &(palg->solvedProblems[0]),palg->problems[m]);
						  palg->num_solved=1;
						  if(print_all)
						  {
							  printf("\nPD_algorithm.c		pD		This solution is the best now. acum: %f",palg->problems[m].solution.acum);
							  //show_aproblem_PD(&(palg->problems[m]));
							  printf("\nPD_algorithm.c		pD		pre update best palg: %f",palg->best);
							  printf("\nPD_algorithm.c		pD		pre update best final_alg: %f", final_alg.best);
						  }
						  update_best(palg,&(palg->solvedProblems[0]) );
						  if(print_all)
						  {
							  printf("\nPD_algorithm.c		pD		post update best");
							  //show_aproblem_PD(&(palg->problems[39]));//TODO check
							  printf("\nPD_algorithm.c		pD		NUM SOLVED: %d",palg->num_solved);
							  printf("\nPD_algorithm.c		pD		post update best palg: %f",palg->best);
							  printf("\nPD_algorithm.c		pD		post update best final_alg: %f", final_alg.best);
							  printf("\nPD_algorithm.c		pD		we are going to copy the problem in solved");
						  }

					  }
					  update_alg_best(palg);
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
						  //for tuple_prune
						  Logico tuple_prune=FALSE;

						  int round=palg->problems[m].index-palg->ppd.index;
						  double my_value=palg->ppd.aproblem.values
								  	  	  	  	  	[
													(round+1)+as[u].indexResource*palg->ppd.aproblem.numTask
													];
						  if(print_all)
						  {
							  printf("\nPD_algorithm.c		pD()		as[u]:%d",as[u].indexResource);
							  printf("\nPD_algorithm.c		pD()		problems[m].index: %d, ppd.index: %d, round %d:",palg->problems[m].index,palg->ppd.index,round);
							  printf("\nPD_algorithm.c		pD()		acum:%f, my_value: %f",palg->problems[m].solution.acum,my_value);
							  for(int i=0;i<palg->problems[m].solution.lengthArrays;i++)
							  {
								  printf("\nPD_algorithm.c		pD()		sol %d:%d",i,palg->problems[m].solution.resources[i].position);
							  }

						  }

						  if(round==1)
						  {
							  if(print_all)
							  {
								  printf("\nPD_algorithm.c		pD()	round==1>>we check prune");
							  }
							  check_prune(
							  		  &tuple_prune,
							  		  &tuple_prune_data,
							  		  &as,
									  &(palg->problems[m]),
							  		  my_value,
							  		  u,
							  		  ismin,
							  		  ismax);

						  }

						  //for dummy prune
						  double b_estimated=get_best_estimate(&(palg->problems[m]));
						  if(print_all)
						  {
							  printf("\nPD_algorithm.c		pD()		my best estimated in problem %d is: %f vs final alg:%f",m,b_estimated,final_alg.best);
						  }
						  if
						  (
								  //dummy prune
								  ((ismin && b_estimated<=final_alg.best)
											  || (ismax && b_estimated>=final_alg.best))

											  &&
								  //tuple prune
											  (!tuple_prune)

						  )//no prune, go on
						  {
							  if(print_all)
							  {
								  printf("\nPD_algorithm.c		pD		NO PRUNE best in palg: %f", palg->best);
								  printf("\nBEST ESTIMATED IN PD var %f\n .IN FINAL ALG BEST %f", b_estimated, final_alg.best);
							  }

							  //case no prune,control our worst is better than global to change it
							  double w_estimated=get_worst_estimate(&(palg->problems[m]));
							  if((w_estimated>final_alg.best && ismax) ||
									  (w_estimated<final_alg.best && ismin)  )
							  {
								  if(print_all)
								  {
									  printf("\nPD_algorithm.c		pD		NO PRUNE AND CHANGE final_alg.best: %f to worst estimated %f", palg->best, w_estimated);

								  }

								  final_alg.best=w_estimated;
								  if(!serial)
								  {
									  send_best(&final_alg);
								  }

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

								  get_subproblem(&(palg->problems[m]), &appdNew, as[u],numSubproblems);
								  if(print_all)
								  {
									  printf("\nPD_algorithm.c		pD		acum post get subproblem father: %f",palg->ppd.solution.acum);
									  printf("\nPD_algorithm.c		pD		acum post get subproblem new m: %f",appdNew.solution.acum);
									  printf("\nPD_algorithm.c		pD		is NOT base case: last appdNew sol: %s\n",appdNew.solution.resources[appdNew.solution.lengthArrays-1].name);
									  printf("\nPD_algorithm.c		pD		i=%d of %d alternatives\n",u, numAlternatives);
								  }

								  copy_aproblem_PD( &(palg->problems[lengthNewArrayAppd]),appdNew);


								  lengthNewArrayAppd++;
								  if(print_all)
								  {
									  printf("\nPD_algorithm.c		pD		adding alt len array problem in PD = %d", lengthNewArrayAppd);
								  }

							  }//end for num subproblem=1
						  }//end if not prune
						  else//prune
						  {
							  if(print_all)
							  {
								  printf("\nPD_algorithm.c		pD		prune");
							  }
							  MPE_Log_event(event6, 0, "prune in PD");

						  }
					  }//end for alternatives

					  palg->num_problems=lengthNewArrayAppd;
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD()		end of not case base solution");
					  }

				  }//end else (not base case)
			  }//end if num alt>0
		  }//end for size
	  }//end BFS
	  else//DFS
	  {
		  int down=0;
		  if(print_all)
		  {
			  printf("\nPD_algorithm.c		pD()	p%d deep",id);
			  printf("\nPD_algorithm.c		pD()	p%d lengthNewArrayAppd: %d",id,lengthNewArrayAppd);
		  }
		  while(lengthNewArrayAppd>0)
		  {
			  int m=lengthNewArrayAppd-1;
			  if(print_all)
			  {
				  printf("\nPD_algorithm.c		pD()	p%d------------------------------m=%d",id,m);
				  //printf("\nPD_algorithm.c		pD()	best final alg=%f",final_alg.best);
			  }

			  ///redistribution
			  if(lengthNewArrayAppd>1 && !serial)
			  {//to not send the only one
				  int rcvr=0;
				  down = waiting_petition(buffer_w,request_w,palg,m,&rcvr);
				  if(print_all)
				  {
					  printf("\nPD_algorithm.c	 pD()		p%d DEEP. more than one problem and asked to send=%d", id,down);
				  }
				  if(down)
				  {
					  add_transfered(&transfered,(&palg->problems[m]),rcvr);
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c	 pD()		add_trasfered rcvr %d",rcvr);
						  show_transfered(&transfered);
					  }
				  }
			  }
			  m=m-down;
			  if(!serial)
			  {
				  waiting_confirming(&transfered);
			  }

			  lengthNewArrayAppd=lengthNewArrayAppd-down;
			  ///////////
			  if(!serial)
			  {
				  waiting_best(buffer, request_b);
			  }
			  update_alg_best(palg);
			  int max_num_problem=get_max_num_problems_deep(&appd);
			  int len=max_num_problem+lengthNewArrayAppd;
/////////////////////////////OK
			  int numPreviousProblems=palg->num_problems;
			  int max_num_alternatives=get_max_num_alternatives(&appd);
			  Alternative as[max_num_alternatives];
			  init_alternative_array(&as,max_num_alternatives);
			  int numAlternatives=get_alternatives(&(palg->problems[m]), as);
			  if(print_all)
			  {
				  printf("\nPD_algorithm.c		pD()	max num sub=%d",max_num_problem);
				  printf("\nPD_algorithm.c		pD()	first len array=%d",lengthNewArrayAppd);
				  printf("\nPD_algorithm.c		pD()	len=%d",len);
				  printf("\nPD_algorithm.c		pD()	num previous=%d",numPreviousProblems);
				  printf("\nPD_algorithm.c		pD()	num max alternatives=%d",max_num_alternatives);
				  printf("\nPD_algorithm.c		pD()	num alternatives=%d",numAlternatives);

			  }

			  if(numAlternatives==0)
			  {
					  //TODO
					//  printf("\nPD_algorithm.c		pD		no alternatives\n");
			  }
			  else
			  {

				  if(is_base_case(&(palg->problems[m])))
				  {
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		p%d before get solution case base",id);
						  //show_aproblem_PD(&(palg->problems[m]));
						  //printf("\nPD_algorithm.c		pD		before get solution case base.best in palg: %f", palg->best);
						  //printf("\nPD_algorithm.c		pD		before get solution case base.best in final_alg: %f", final_alg.best);
					  }

					  SpPD sp;
					  get_solution_base_case((&(palg->problems[m])),&sp);
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		p%d case base post get solution",id);
//						  printf("\nPD_algorithm.c		pD		post get solution case base. best palg: %f",palg->best);
//						  printf("\nPD_algorithm.c		pD		post get solution case base. best final: %f",final_alg.best);
//						  printf("\nPD_algorithm.c		pD		in base case. acum: %f",palg->problems[m].solution.acum);
					  }

					  if(palg->num_solved>0 && palg->problems[m].solution.acum==palg->solvedProblems[0].solution.acum)//more than one solution
					  {
						  copy_aproblem_PD( &(palg->solvedProblems[palg->num_solved]),palg->problems[m]);
						  palg->num_solved++;

					  }
					  else if(palg->num_solved==0 ||
							  (palg->num_solved>0 && palg->problems[m].solution.acum>palg->solvedProblems[0].solution.acum))//now this is the only one best solution
					  {
						  copy_aproblem_PD( &(palg->solvedProblems[0]),palg->problems[m]);
						  palg->num_solved=1;
						  if(print_all)
						  {
							  printf("\nPD_algorithm.c		pD		This solution is the best now. acum: %f",palg->problems[m].solution.acum);
							  show_aproblem_PD(&(palg->problems[m]));
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

					  }

					  lengthNewArrayAppd--;
					  //(palg->num_problems)--;
					  update_alg_best(palg);
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		is base case and takes alternative: %d\n", sp.alternative.indexResource);
						  printf("\nPD_algorithm.c		pD	lengthewarray=%d",lengthNewArrayAppd);
						  printf("\nPD_algorithm.c		pD	palg->num problems=%d",(palg->num_problems));

					  }
				  }
				  else//not case base
				  {
					  ////////////////////////////////////////////////DEEP not case base
					  //get new problems
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD		Not case base. Problem: ");
						  show_aproblem_PD(&(palg->problems[m]));
						  printf("\nPD_algorithm.c		pD		Not case base. Alternatives: ");
						  for(int k=0;k<numAlternatives;k++)
						  {
							  printf("%d ", as[k].indexResource);
						  }
						  printf("\n");
					  }

					  randomize(palg,as);//now not using
					  Logico ismin;
					  Logico ismax;
					  AproblemPD aux[numAlternatives];//TODO to include more than one problem for each alternative
					  int len_aux=0;
					  int count=0;
					  for(int u=0;u<numAlternatives;u++)
					  {
						  //prune control
						  ismin=is_min(palg);
						  ismax=is_max(palg);
						  //for tuple_prune
//						  Logico tuple_prune=FALSE;
//
//						  int round=palg->problems[m].index-palg->ppd.index;
//						  double my_value=palg->ppd.aproblem.values
//								  	  	  	  	  	[
//													(round+1)+as[u].indexResource*palg->ppd.aproblem.numTask
//													];
						  if(print_all)
						  {
							  printf("\nPD_algorithm.c		pD()		p%d as[u]:%d++++++++++++++",id,as[u].indexResource);
//							  printf("\nPD_algorithm.c		pD()		problems[m].index: %d, ppd.index: %d, round %d:",palg->problems[m].index,palg->ppd.index,round);
//							  printf("\nPD_algorithm.c		pD()		acum:%f, my_value: %f",palg->problems[m].solution.acum,my_value);
//							  for(int i=0;i<palg->problems[m].solution.lengthArrays;i++)
//							  {
//								  printf("\nPD_algorithm.c		pD()		sol %d:%d",i,palg->problems[m].solution.resources[i].position);
//							  }
							}
//									  //for dummy prune
						 double b_estimated=get_best_estimate(&(palg->problems[m]));
						 if(print_all)
						 {
							 printf("\nPD_algorithm.c		pD()	b_estimated: %f vs final alg: %f",b_estimated,final_alg.best);
						 }
						  if
						  (
								  //dummy prune
								  ((ismin && b_estimated<=final_alg.best)
											  || (ismax && b_estimated>=final_alg.best))

//											  &&
								  //tuple prune
//											  (!tuple_prune)

						  )//no prune, go on
						  {
							  //case no prune,control our worst is better than global to change it
							  double w_estimated=get_worst_estimate(&(palg->problems[m]));
							  if((w_estimated>final_alg.best && ismax) ||
									  (w_estimated<final_alg.best && ismin)  )
							  {
								  if(print_all)
								  {
									  printf("\nPD_algorithm.c		pD		NO PRUNE AND CHANGE final_alg.best: %f to worst estimated %f", palg->best, w_estimated);

								  }

								  final_alg.best=w_estimated;
								  if(!serial)
								  {
									  send_best(&final_alg);
								  }
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

								  get_subproblem(&(palg->problems[m]), &appdNew, as[u],numSubproblems);
								  if(count==0)
								  {
									  //save_for_on_top &appdNew,
									  copy_aproblem_PD(&appd_for_top_deep,appdNew);
									  count++;
									  if(print_all)
									  {
										  printf("\nPD_algorithm.c	pD()	appd for top:");
										  show_aproblem_PD(&appd_for_top_deep);
									  }
								  }
								  else
								  {
									  copy_aproblem_PD( &(palg->problems[lengthNewArrayAppd-1+count]),appdNew);

									  if(print_all)
									  {
										  printf("\nPD_algorithm.c	pD()	copy on alg[%d]:",lengthNewArrayAppd-1+count);
										  show_aproblem_PD(&(palg->problems[lengthNewArrayAppd-1+count]));
									  }
									  count++;
								  }
//								  copy_aproblem_PD( &aux[len_aux],appdNew);
								  len_aux++;
								  if(print_all)
								  {
									  printf("\nPD_algorithm.c		pD		p%d %d of %d subproblems. len aux:%d",id,j, numSubproblems,len_aux);
									  printf("\nPD_algorithm.c		pD		p%d acum post get subproblem father acum: %f",id,palg->problems[m].solution.acum);
									  printf("\nPD_algorithm.c		pD		acum post get subproblem new m: %f",appdNew.solution.acum);
									  printf("\nPD_algorithm.c		pD		is NOT base case: last appdNew sol: %s\n",appdNew.solution.resources[appdNew.solution.lengthArrays-1].name);
									  printf("\nPD_algorithm.c		pD		i=%d of %d alternatives\n",u, numAlternatives);
//									  printf("\nPD_algorithm.c		pD		show aux problem:");
//									  show_aproblem_PD(&(aux[len_aux-1]));
								  }
								  //lengthNewArrayAppd++;
							  }//end for num subproblem=1
						  }//end if not prune
						  else//prune
						  {
							  if(print_all)
							  {
								  printf("*******************************prune!!!!!");
							  }

							  MPE_Log_event(event6, 0, "prune in PD");

						  }
					  }//end for alternatives
//					  for(int v=0;v<len_aux;v++)
//					  {
//						  //we must copy over the father,
//						  copy_aproblem_PD( &(palg->problems[lengthNewArrayAppd-1+v]),aux[v]);
//						  if(print_all)
//						  {
//							  printf("\nPD_algorithm.c		pD		show new %d problem copied in problems in pos:%d",v,lengthNewArrayAppd-1+v);
//							  //show_aproblem_PD(&(palg->problems[lengthNewArrayAppd-1+v]));
//
//						  }
//
//					  }
					  copy_aproblem_PD( &(palg->problems[m]),appd_for_top_deep);

					  lengthNewArrayAppd=lengthNewArrayAppd+len_aux-1;
					  palg->num_problems=lengthNewArrayAppd;
					  if(print_all)
					  {
						  printf("\nPD_algorithm.c		pD()	new len=%d",lengthNewArrayAppd);
						  printf("\nPD_algorithm.c		pD()		end of not case base solution");
					  }
				  }//end else (not base case)
			  }//end if num alt>0
		  }//end for size
	  }//end deep

	  //redistribution
	  if(!serial){
		  if(print_all)
		  {
			  printf("\nPD_algorithm.c	pD()		----------------There are %d transfered not confirmed", transfered.len_transfered);
		  }
		  if(transfered.len_transfered>0)
		  {
			  for(int j=0;j<transfered.len_transfered;j++)//no confirmed
			  {
				  	  copy_aproblem_PD(&(palg->problems[j]),transfered.transfered[j]);
					  //palg->problems[j]=transfered.transfered[j];
					  palg->num_problems=transfered.len_transfered;
					  if(print_all)
					  {
						  int myid;
						  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
						  printf("\nPD_algorithm.c	pD()		transfered no confirmed in process %d. transfered and palg:",myid);
						  show_aproblem_PD(&(transfered.transfered[j]));
						  show_aproblem_PD(&(palg->problems[j]));
					  }


			  }
			  transfered.len_transfered=0;
			  pD(palg,buffer,request_b,buffer_w,request_w);
		  }
	  }


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
	  delete_problem_PD(&(palg->ppd));
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

