/*
 * problem_MPI.c
 *
 *  Created on: 13 ene. 2021
 *      Author: practica
 */

#include "problem_MPI.h"
#include <stddef.h>

static int pD_distribution(PalgorithmPD palg);



int distribution(PalgorithmPD palg, int num_processes)
{
	int res=-1;

	Solution sol;
	do
	{
		for(int i=0;i<get_num_subproblems();i++)
		{
			pD_distribution(palg);
		}
	}while(palg->isRandomize && get_PDsolution(palg,&sol)!=0);

	if(palg->num_problems<=num_processes)
	{
		for(int i=1; i<palg->num_problems+1;i++)
		{
			send_work(palg,i,i);//TODO
		}
	}
	else{
		int j;
		for(int i=0; i<num_processes;i++)
		{
			send_work(palg,j,i);//TODO now is only one round
			j++;
		}
	}


	return res;


}

int rcv_work()
{
	int res=0;
	int tag_problem=1;
	int ierr;

	printf("rcv...\n");
	struct Work work;
	MPI_Datatype work_mpi_datatype;
	//MPI_Type_contiguous ( 3, MPI_INT, &work_mpi_datatype );
	const int nitems=4;
	int          blocklengths[4] = {1,1,1,50};
	MPI_Datatype types[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_DOUBLE};

	//MPI_Aint     offsets[nitems];
	const MPI_Aint offsets[4]= { 0, sizeof(int), sizeof(int)*2,sizeof(int)*3};

//	offsets[0] = offsetof(struct Work, num_tasks);
//	offsets[1] = offsetof(struct Work, num_resources);
//	offsets[2] = offsetof(struct Work, alternative);
//	offsets[3] = offsetof(struct Work, url);
//	printf("offsets of work.url %d", offsets[3]);
//	offsets[3] = offsetof(Cadena, url);
//	printf("offsets of cadena %d", offsets[3]);

	MPI_Type_create_struct(nitems, blocklengths, offsets, types,  &work_mpi_datatype);

	MPI_Type_commit ( &work_mpi_datatype );
	// ierr = MPI_Send ( &work, 1, work_mpi_datatype, num_process, tag, MPI_COMM_WORLD );
	MPI_Recv ( &work, 1, work_mpi_datatype, MPI_ANY_SOURCE, tag_problem, MPI_COMM_WORLD,MPI_STATUS_IGNORE);

	printf("%d resources\n",work.num_resources);
	printf("%f send first\n",work.values[0]);
	printf("%f send 8\n",work.values[8]);
	printf("%f send last\n",work.values[50]);
	//ierr = MPI_TYPE_FREE(&work_mpi_datatype);
	return res;
}

int send_work(PalgorithmPD palg,int num_problem, int num_process)
{
	int res=0;
	int tag=1;
	int ierr;
	//char ch="h";
	Cadena c="/home/practica/eclipse-workspace/c/files/data3.txt";
	printf("send work to %d\n",num_process);
	show_aproblem(&(palg->ppd.aproblem));
	//TODO include num_problem
	struct Work work;
	work.num_resources=palg->ppd.aproblem.numResources;
	work.num_tasks=palg->ppd.aproblem.numTask;
	work.alternative=1;//todo
	int num_values=palg->ppd.aproblem.numResources*palg->ppd.aproblem.numTask;
	for(int i=0;i<num_values;i++)
	{
		work.values[i]=palg->ppd.aproblem.values[i];
	}
//    strncpy(work.url, "/home/practica/eclipse-workspace/c/files/data3.txt", 255);
//    work.url[255] = '\0';
	//strcpy(work.url,c);
	//work.url=ch;
	//create_datatype_work_MPI
	MPI_Datatype work_mpi_datatype;
	//	MPI_Type_contiguous ( 3, MPI_INT, &work_mpi_datatype );
	//	MPI_Type_commit ( &work_mpi_datatype );

	//another way for more complex structs
	    const int nitems=4;
	    int          blocklengths[4] = {1,1,1,50};
	    MPI_Datatype types[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_DOUBLE};

//	    MPI_Aint     offsets[nitems];
//
//	    offsets[0] = offsetof(struct Work, num_tasks);
//	    offsets[1] = offsetof(struct Work, num_resources);
//	    offsets[2] = offsetof(struct Work, alternative);
//	    offsets[3] = offsetof(struct Work, url);
	    const MPI_Aint offsets[4]= { 0, sizeof(int), sizeof(int)*2,sizeof(int)*3};
	    MPI_Type_create_struct(4, blocklengths, offsets, types,  &work_mpi_datatype);
	    MPI_Type_commit( &work_mpi_datatype);

    ierr = MPI_Send ( &work, 1, work_mpi_datatype, num_process, tag, MPI_COMM_WORLD );
    //ierr = MPI_TYPE_FREE( &work_mpi_datatype);
	return res;
}

void waitting_answer()
{

}

int pD_distribution(PalgorithmPD palg)
{
	 	  int res=0;
		  AproblemPD appd=palg->ppd;
		  AproblemPD newArrayAppd[get_max_num_problems(&appd)];
		  int lengthNewArrayAppd=0;
		  int max_num_problem=get_max_num_problems(&appd);
		  AproblemPD problems[max_num_problem];
		  int numPreviousProblems=getPreviousProblems(palg, problems);
		  Alternative * as;
		  init_alternative_array(&as,get_max_num_alternatives(&appd));
		  //as=(Alternative*)malloc(sizeof(Alternative)*50);//TODO
		  int numAlternatives=get_alternatives(&appd, as);
		  //delete_alternatives(&as);
		  if(numAlternatives==0)
		  {
				  //TODO
				  printf("\n no alternatives\n");
		  }
		  else
		  {

			  if(is_base_case(&problems[0]))
			  {
				  //TODO
				  				  printf("\n too little\n");
			  }

			  else
			  {
				  //get new problems
				  printf("        Alternatives: ");
				  for(int k=0;k<numAlternatives;k++)
				  {
					  printf("%d ", as[k].indexResource);
				  }
				  printf("\n");
				  randomize(palg,as);//not using
				  Logico ismin;
				  Logico ismax;
				  for(int u=0;u<numAlternatives;u++)
				  {
					  //prune
					  ismin=is_min(palg);
					  ismax=is_max(palg);
					  double estimated=get_estimate(&problems[0]);
					  if((ismin && estimated<=palg->best)
										  || (ismax && estimated>=palg->best))
					  {
						  int numSubproblems=get_num_subproblems();
						  AproblemPD appdNew;
						  for(int j=0;j<numSubproblems;j++)
						  {
							  initAProblemPD(&appdNew,&(palg->ppd.aproblem));

							  get_subproblem(&problems[0], &appdNew, as[u],numSubproblems);
							  printf("\n     is NOT base case: last appdNew sol: %s\n",appdNew.solution.resources[appdNew.solution.lengthArrays-1].name);
							  printf("     i=%d of %d alternatives\n",u, numAlternatives);
							  //if problem//TODO
									  //add problem to new array
							  copy_aproblem_PD( &(newArrayAppd[lengthNewArrayAppd]),appdNew);
							 lengthNewArrayAppd++;

						  }//end for num subproblem=1
					  }//end if not prune
				   }//end for alternatives
			  }//end else (not base case)
			}//end else (exits alternative)

		  for(int w=0; w<lengthNewArrayAppd;w++)
		  {
			  palg->problems[w]=newArrayAppd[w];
		  }
		  palg->num_problems=lengthNewArrayAppd;

  return res;
}
