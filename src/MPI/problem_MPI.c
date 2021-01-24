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

	if(palg->num_problems==num_processes)
	{
		for(int i=1; i<palg->num_problems+1;i++)
		{
			send_work(palg,i,1,i);//first i is alternative, second one is process target
			//TODO process 0 resolves 0. look for not blocking solution
		}
	}
	else if(palg->num_problems<num_processes)
	{
		int i;
		for(i=1; i==palg->num_problems;i++)
		{
			send_work(palg,i,1,i);

		}
		for(int j=i;j<num_processes;j++)
		{
			send_work(palg,i,0,j);
		}
	}
	else//num problems >num processes
	{
		for(int i=1; i==num_processes;i++)
		{
			send_work(palg,i,1,i);//TODO num works
		}

	}


	return res;


}

int rcv_work()
{
	int res=0;
	int tag_work=0;
	int tag_values=1;
	int tag_tasks=2;
	int tag_resources=3;
	int ierr=0;

	printf("rcv...\n");
	struct Work work;


	//getting work

	MPI_Datatype work_mpi_datatype;
	int blocklengths[4] = {1,1,1,1};
	MPI_Datatype types[4] = {MPI_INT, MPI_INT,MPI_INT, MPI_INT};
	const MPI_Aint offsets[4]= { 0, sizeof(int),sizeof(int)*2,sizeof(int)*3};
	MPI_Type_create_struct(4, blocklengths, offsets, types,  &work_mpi_datatype);
	MPI_Type_commit ( &work_mpi_datatype );
	MPI_Status status;
    MPI_Recv(&work, 1, work_mpi_datatype , 0, tag_work, MPI_COMM_WORLD, &status);

    int count;
    MPI_Get_count(&status, work_mpi_datatype, &count);
    if(count>0)
    {

		printf("\n received num task: %d",work.num_tasks);
		printf("\n received num resources: %d\n",work.num_resources);
		int size_values=work.num_tasks*work.num_resources;


		//getting values

		double values[size_values];

		MPI_Recv ( &values, size_values, MPI_DOUBLE, MPI_ANY_SOURCE, tag_values, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf("%d resources\n",work.num_resources);
		printf("%d received size values\n",size_values);
		printf("%d received type\n",work.type);
		printf("%f received first\n",values[0]);
		printf("%f received second\n",values[1]);

		//getting tasks and resources names

		char serialized[1000];//TODO
		Task tasks[work.num_tasks];

		char serialized_resources[1000];//TODO
		Resource resources[work.num_resources];

		MPI_Recv( &serialized, 1000, MPI_CHAR, MPI_ANY_SOURCE, tag_tasks, MPI_COMM_WORLD,MPI_STATUS_IGNORE );
		deserializer_tasks(&serialized,work.num_tasks,tasks);
		printf("*************************POST RCVE TASKS name 3º task\n%s\n",tasks[2].name);
		MPI_Recv( &serialized_resources, 1000, MPI_CHAR, MPI_ANY_SOURCE, tag_resources, MPI_COMM_WORLD,MPI_STATUS_IGNORE );
		deserializer_resources(&serialized_resources,work.num_resources,resources);
		printf("*************************POST RCVE RESOURCES name 3º resource\n%s\n",resources[2].name);

		//create problem
		PAproblem pa;
		init_aproblem(pa,tasks,resources,work.num_tasks, work.num_resources, values);
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		show_aproblem(pa);
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		init_work(pa, work.alternative);

    }//end if load!=FALSE
    MPI_Type_free ( &work_mpi_datatype);
	return res;
}

int init_work(PAproblem pa, int alternative)
{
	int res=0;
	AproblemPD appd;
	initAProblemPD(&appd, pa);
	AlgorithmPD alg;
	init_algorithmPD(&alg, appd);
	printf("best post init: %f",alg.best);
	show_aproblem(&(alg.ppd.aproblem));
	//update problem with alternative
	alg.ppd.index=1;
	strcpy(alg.ppd.solution.resources[alg.ppd.solution.lengthArrays].name, pa->resources[alternative].name);
	alg.ppd.solution.lengthArrays=alg.ppd.solution.lengthArrays+1;
	alg.ppd.solution.acum=pa->values[alternative];
	alg.problems[0]=alg.ppd;
	printf("Assert index =1 in problems post work:%d ",alg.problems[0].index);

	exec_algorithm(&alg);
	printf("\nAfter exec algoritm\n");
	Solution sol;
	get_PDsolution(&alg, &sol);
	for(int i=0;i<alg.ppd.solution.lengthArrays;i++){
		printf("\nResources: \n*%s\n",alg.ppd.solution.resources[i].name);
	}
	printf("Solution value: %f", alg.ppd.solution.acum);
	delete_algorithmPD(&alg);

	return res;
}
int send_work(PalgorithmPD palg,int alternative, int num_alternatives, int num_process)
{
	printf("\n...................sending alternative %d to process %d\n",alternative,num_process);
	int res=0;

	int tag_work=0;
	int tag_values=1;
	int tag_tasks=2;
	int tag_resources=3;

	printf("send work to %d\n",num_process);
	show_aproblem(&(palg->ppd.aproblem));

	struct Work work;
	char serialized_tasks[1000];//TODO
	int len=serializer_tasks(palg, &serialized_tasks);
	char serialized_resources[1000];//TODO
	int len_resources=serializer_resources(palg, &serialized_resources);

	work.num_resources=palg->ppd.aproblem.numResources;
	work.num_tasks=palg->ppd.aproblem.numTask;
	work.alternative=alternative;//TODO
	work.type=palg->ppd.aproblem.type;
	int num_values=work.num_resources*work.num_tasks;
	printf("\n In work task: %d",work.num_tasks);
	printf("\n In work resources: %d",work.num_resources);

	double values[num_values];
	Cadena tasks[num_values];
	Cadena resources[num_values];

	for(int i=0;i<num_values;i++)
	{
		values[i]=palg->ppd.aproblem.values[i];
		printf("inside send. value[%d]=%f\n",i,values[i]);
	}
	for(int i=0;i<work.num_resources;i++)
	{
		strcpy(resources[i],palg->ppd.aproblem.resources[i].name);
		printf("inside send. resources[%d]=%s\n",i,resources[i]);
	}
	for(int i=0;i<work.num_tasks;i++)
	{
		strcpy(tasks[i],palg->ppd.aproblem.tasks[i].name);
		printf("inside send. resources[%d]=%s\n",i,tasks[i]);
	}

	//sending
	MPI_Datatype work_mpi_datatype;
	int blocklengths[4] = {1,1,1,1};
	MPI_Datatype types[4] = {MPI_INT, MPI_INT,MPI_INT, MPI_INT};
    const MPI_Aint offsets[4]= { 0, sizeof(int),sizeof(int)*2, sizeof(int)*3};
	MPI_Type_create_struct(4, blocklengths, offsets, types,  &work_mpi_datatype);
	MPI_Type_commit ( &work_mpi_datatype);

	printf("\n sending num task: %d",work.num_tasks);
	printf("\n sending num resources: %d\n",work.num_resources);

    MPI_Send(&work, num_alternatives, work_mpi_datatype, num_process, tag_work, MPI_COMM_WORLD);
    MPI_Type_free( &work_mpi_datatype);

	//sending dinamic arrays

	MPI_Send( &values, num_values, MPI_DOUBLE, num_process, tag_values, MPI_COMM_WORLD );
	MPI_Send( &serialized_tasks, len, MPI_CHAR, num_process, tag_tasks, MPI_COMM_WORLD );
	MPI_Send( &serialized_resources, len_resources, MPI_CHAR, num_process, tag_resources, MPI_COMM_WORLD );

	return res;
}


int serializer_tasks(PalgorithmPD palg, char* all)
{
	Cadena temp="";
	Cadena divisor=";";

	for(int i=0;i<palg->ppd.aproblem.numTask;i++)
	{
		strcat(temp,palg->ppd.aproblem.tasks[i].name);
		strcat(temp,divisor);
		printf("\n----%s\n",temp);
	}
	strcpy(all,temp);
	printf("\n---->>>>%s\n",all);
	int len;
	for (len = 0; all[len] != '\0'; ++len);
	printf("Length of Str tasks is %d", len);
	return len;
}
int deserializer_tasks(char* all, int len, PTask tasks)
{

	Cadena aux="";
	char divisor=';';
	printf("\nINSIDE DESERIALIZED---->>>>%s\n",all);
	int res=0;
	int count=0;
	for(int i=0;i<1000;i++)//TODO
	{
		if(all[i]==divisor)
		{
			strcpy(tasks[count].name,aux);
			count++;
			strcpy(aux,"");
			if(count==len)
			{
				printf("\nfound %d *\n",count);
				break;
			}

		}
		else{
			strncat(aux, &all[i], 1);

		}

	}
	return res;
}
int serializer_resources(PalgorithmPD palg, char* all)
{
	Cadena temp="";
	Cadena divisor=";";

	for(int i=0;i<palg->ppd.aproblem.numResources;i++)
	{
		strcat(temp,palg->ppd.aproblem.resources[i].name);
		strcat(temp,divisor);
	}
	strcpy(all,temp);
	printf("\nRESOURCES---->>>>%s\n",all);
	int len;
	for (len = 0; all[len] != '\0'; ++len);
	printf("Length of Str resources is %d", len);
	return len;
}
int deserializer_resources(char* all, int len, PResource resources)
{

	Cadena aux="";
	char divisor=';';
	printf("\nINSIDE DESERIALIZED  resources---->>>>%s\n",all);
	int res=0;
	int count=0;
	for(int i=0;i<1000;i++)//TODO
	{
		if(all[i]==divisor)
		{
			strcpy(resources[count].name,aux);
			count++;
			strcpy(aux,"");
			if(count==len)
			{
				printf("\nfound resources %d *\n",count);
				break;
			}

		}
		else{
			strncat(aux, &all[i], 1);

		}

	}
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
