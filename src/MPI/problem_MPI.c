/*
 * problem_MPI.c
 *
 *  Created on: 13 ene. 2021
 *      Author: practica
 */

#include "problem_MPI.h"
#include <stddef.h>

static int pD_distribution(PalgorithmPD palg);


//we will free process 0 from problem resolution.Process 0 will show grafics
int distribution(PalgorithmPD palg)
{
	int res=-1;
	Solution sol;
	int num_slaves=numprocs-1;
    MPI_Request request;
    MPI_Status status;
    int flag;




	printf("\n num problems-num slaves: %d-%d",palg->num_problems,num_slaves);
	do
	{
		printf("\n num subpr: %d",get_num_subproblems());
		for(int i=0;i<get_num_subproblems();i++)
		{
			pD_distribution(palg);
		}
	}while(palg->isRandomize && get_PDsolution(palg,&sol)!=0);
	printf("\n num problems-num slaves: %d-%d",palg->num_problems,num_slaves);


	if(palg->num_problems==num_slaves)
	{
		printf("\nif");

		for(int i=1; i<(num_slaves+1);i++)
		{
			printf("inside for");
			int alternatives[1];
			alternatives[0]=i-1;
			printf("\n num problems==num slaves: alternative->%d",alternatives[0]);
			send_work(palg,&alternatives,1,i);//sending 1 alternative to process i

		}
	}
	else if(palg->num_problems<num_slaves)
	{
		printf("\n else if");
		int i;
		for(i=1; i<(palg->num_problems+1);i++)
		{
			int alternatives[1];
			alternatives[0]=i-1;
			printf("\n num problems<num slaves: alternative->%d",alternatives[0]);
			send_work(palg,&alternatives,1,i);

		}
		for(int j=palg->num_problems+1;j<num_slaves+1;j++)
		{
			int alternatives[0];
			send_work(palg,&alternatives,0,j);
		}
	}
	else//num problems >num processes
	{

		int rounds=(palg->num_problems)/num_slaves;
		int more_round=(palg->num_problems)-num_slaves*rounds;

		for(int i=1; i<(more_round+1);i++)
		{
			printf("\n0000000000000000000000000000000000000000000000000000sending case num problems >num processes. resource 0: %s",palg->ppd.aproblem.resources[0].name);
			int alternatives[100];//TODO
			for(int j=0;j<rounds+1;j++)
			{
				printf("\n1111111111111111111111111111111111111111111111111111sending case num problems >num processes. resource 0: %s",palg->ppd.aproblem.resources[0].name);
				int a=(i-1)+(j*num_slaves);
				printf("\nalternative %d=%d",j,a);
				alternatives[j]=a;//process i, distribution round j
			}
			send_work(palg,&alternatives,rounds+1,i);
		}
		for(int i=more_round+1;i<num_slaves+1;i++)
		{
			int alternatives[100];//TODO
			for(int j=0;j<rounds;j++)
			{
				alternatives[j]=(i-1)+(j*num_slaves);//process i, distribution round j
			}
			send_work(palg,alternatives,rounds,i);
		}

	}

	printf("00000000000000000000000000 outgoing distribution");
	return res;


}

int rcv_work()
{
	MPE_Log_event(event2a, 0, "start receive work");
	int res=0;
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

////    int count;
////    MPI_Get_count(&status, work_mpi_datatype, &count);
    int alternatives[work.num_alternatives];
    if(work.num_alternatives>0)
    {
    	MPI_Recv ( &alternatives, work.num_alternatives, MPI_INT, MPI_ANY_SOURCE, tag_alternatives, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }

    MPI_Type_free ( &work_mpi_datatype);
	//create problem
    MPE_Log_event(event2b, 0, "end receive work");
	Aproblem a;
	init_aproblem(&a,tasks,resources,work.num_tasks, work.num_resources, values);//TODO


	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	show_aproblem(&a);
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");


    init_work(&a, work.num_alternatives, &alternatives);//TODO testing mpe
    printf("\n outgoing rcv_work");


////    printf("\n Inside RCV. num alternatives: %d", work.num_alternatives);
////    for(int a=0;a<work.num_alternatives;a++)
////    {
////    	printf("\n alternative %d=%d", a,alternatives[a]);
////    }
   printf("+\n++++++++++++++++++++++++++++END RCV_work");
	return res;
}

int ask_work()
{
	MPE_Log_event(event1, 0, "ask work");
	int n=0;
	int master=0;
	MPI_Request request;
	MPI_Isend(&n, 1, MPI_INT, master, tag_ask_work, MPI_COMM_WORLD,&request);
	return 0;
}

int rcv_resolved()
{
	MPE_Log_event(event6a, 0, "start receive resolved");
	printf("+\n+++++++++++++++++++++++++++++++++++++++++++++++++++\nINSIDE RCV_REVOLVED\n");
	int res=0;
	int ierr=0;

	struct Resolved resolved;


	//getting

	MPI_Datatype resolved_mpi_datatype;
	int blocklengths[3] = {1,1,1};
	MPI_Datatype types[3] = {MPI_DOUBLE,MPI_INT,MPI_DOUBLE};
	const MPI_Aint offsets[3]= { 0, sizeof(double),sizeof(int)*100+sizeof(double)};

	MPI_Type_create_struct(3, blocklengths, offsets, types,  &resolved_mpi_datatype);
	MPI_Type_commit ( &resolved_mpi_datatype );
	MPI_Status status;
	//rcve every process
	for(int p=1;p<numprocs;p++)
	{
		printf("+\n+++++++++++++++++++++++++++++++++++++++++++++++++++"
				"\npre mpi_rcev\n");

		MPI_Recv(&resolved, 1, resolved_mpi_datatype , p, tag_resolved, MPI_COMM_WORLD, &status);
		printf("");
		if(resolved.num_resolved>0)//TODO
		{
			//palg->best=resolved.value;
			printf("\n ONE BEST IN MASTER-> %f",resolved.value[0]);
			for(int i=0; i<resolved.num_resolved;i++)
				{
					if(resolved.value[i]>=final_alg.best)//TODO
					{
						final_alg.best=resolved.value[i];
						for(int j=0;j<final_alg.ppd.solution.lengthArrays;j++)
						{
							final_alg.solvedProblems[i].solution.resources[j].position=resolved.resources[j];

						}
						final_alg.num_solved=resolved.num_resolved;
					}
				}


		}

		printf("+\n+++++++++++++++++++++++++++++++++++++++++++++++++++\npost mpi_rcev\n"
				"******************received resolved from %d\n ",p);
	}



    MPI_Type_free ( &resolved_mpi_datatype);
    MPE_Log_event(event6b, 0, "end receive resolved");
    printf("+\n+++++++++++++++++++++++++++++++++++++++++++++++++++\nEND RCV RESOLVED\n");
	return res;

}

int init_work(PAproblem pa, int num_alternatives, int * alternatives)
{
	int res=0;
	AproblemPD appd;
	initAProblemPD(&appd, pa);
	AlgorithmPD alg;
	init_algorithmPD(&alg, appd);
	printf("best post init: %f",alg.best);
	show_aproblem(&(alg.ppd.aproblem));

	if (num_alternatives==0){
		ask_work();
		printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
		//recive_work?
		alg.num_solved=0;//TODO TEST
		send_resolved(&alg);//TODO
	}
	else
	{
		if(num_alternatives==1)
		{
			printf("\n	===========only 1 alternative");
			alg.ppd.index=1;
			strcpy(alg.ppd.solution.resources[alg.ppd.solution.lengthArrays].name, pa->resources[alternatives[0]].name);
			alg.ppd.solution.lengthArrays=alg.ppd.solution.lengthArrays+1;
			alg.ppd.solution.acum=pa->values[0+alternatives[0]*alg.ppd.aproblem.numTask];
			alg.problems[0]=alg.ppd;
			printf("Assert index =1 in problems post work:%d ",alg.problems[0].index);
		}
		else//num_alternatives>1
		{
			// we need to add subproblems
			//int get_subproblem(const PAproblemPD, PAproblemPD,Alternative, int);
			//int init_alternative(PAlternative a,const int i)
			AproblemPD subproblems[num_alternatives];
			Alternative alt[num_alternatives];

			for(int i=0;i<num_alternatives;i++)
			{
				alg.ppd.index=0;
				init_alternative(&alt[i],alternatives[i]);
				printf("\n	=============== Alternative: %d\n",alt[i].indexResource);
				printf("\n PPD index: %d\n",alg.ppd.index);
				get_subproblem(&alg.ppd, &subproblems[i],alt[i], get_num_subproblems());

				printf("\n "
						"========================Inside init work more than 1 alternative");
				show_aproblem_PD(&subproblems[i]);
				printf("\n post get subproblems index: %d",subproblems[i].index);
				printf("\n post get subproblems solution.len: %d",subproblems[i].solution.lengthArrays);
				printf("\n post get subproblems solution.acum: %f",subproblems[i].solution.acum);
				printf("\n post get subproblems first resource: %s",subproblems[i].solution.resources[0].name);
				alg.problems[i]=subproblems[i];
				printf("\n post get subproblems index: %d",subproblems[i].index);
				printf("\n post get subproblems solution.len: %d",alg.problems[i].solution.lengthArrays);
				printf("\n post get subproblems solution.acum: %f",alg.problems[i].solution.acum);
				printf("\n post get subproblems first resource: %s",alg.problems[i].solution.resources[0].name);

				if(i>0)
				{
					printf("\nProblem i-1:\n");
					show_aproblem_PD(&(alg.problems[i-1]));
				}

				strcpy(alg.problems[i].solution.resources[alg.problems[i].solution.lengthArrays].name, pa->resources[alternatives[i]].name);
				printf("\nlen solution %dccccccccccccccccccccccccccccc",alg.problems[i].solution.lengthArrays);
				printf("\nacum solution %dccccccccccccccccccccccccccccc",alg.problems[i].solution.acum);
				printf("Assert index =1 in problems post work:%d \n",alg.problems[i].index);

			}
			alg.ppd.index=1;
			alg.num_problems=num_alternatives;//TODO
		}
		for(int i=0;i<alg.num_problems;i++){
			show_aproblem_PD(&(alg.problems[i]));
		}

		printf("\nBefore exec algoritm\n");
		exec_algorithm(&alg);
		printf("\nAfter exec algoritm\n");
		Solution sol;
		get_PDsolution(&alg, &sol);
		for(int i=0;i<alg.ppd.solution.lengthArrays;i++){
			printf("\nResources: \n*%s\n",alg.ppd.solution.resources[i].name);
		}
		printf("Solution value: %f", alg.ppd.solution.acum);
		send_resolved(&alg);
		printf("\nSEND RESOLVED TO MASTER");
		delete_algorithmPD(&alg);
	}//end else (alternative>0)
	printf("\noutgoing initwork");
	return res;
}
int send_work(const PalgorithmPD palg,int *alternatives, int num_alternatives, int num_process)
{
	MPE_Log_event(event1a, 0, "start send");
	printf("\n...................sending  %d  alternatives to process %d\n",num_alternatives,num_process);

	show_aproblem(&(palg->ppd.aproblem));

	int res=0;

	//process 0 resolves 0. Var for Not blocking solution
	MPI_Request request;
	MPI_Status  status;
	int request_complete = 0;

	if(num_process==0){
		return 0;
//		MPI_Isend(palg, 1, MPI_INT, target, tag, MPI_COMM_WORLD, &request);
//					  // Here we do some work while waiting for process 1 to be ready
//					  while (has_work)
//					  {
//						do_work();
//						// We only test if the request is not already fulfilled
//						if (!request_complete)
//						{
//							MPI_Test(&request, &request_complete, &status);
//						}
//					  }
//
	}



	struct Work work;
	char serialized_tasks[1000];//TODO
	int len=serializer_tasks(palg, &serialized_tasks);
	char serialized_resources[1000];//TODO

	int len_resources=serializer_resources(palg, &serialized_resources);

	work.num_resources=palg->ppd.aproblem.numResources;
	work.num_tasks=palg->ppd.aproblem.numTask;
	work.num_alternatives=num_alternatives;//TODO
	work.type=palg->ppd.aproblem.type;
	int num_values=work.num_resources*work.num_tasks;
	printf("\n In work task: %d",work.num_tasks);
	printf("\n In work resources: %d",work.num_resources);

	double values[num_values];
	int alternatives_to_work[num_alternatives];
	Cadena tasks[num_values];
	Cadena resources[num_values];

	for(int i=0;i<num_values;i++)
	{
		values[i]=palg->ppd.aproblem.values[i];
		printf("inside send. value[%d]=%f\n",i,values[i]);
	}
	for(int i=0;i<num_alternatives;i++)
	{
		alternatives_to_work[i]=alternatives[i];//TODO
		printf("inside send. alternative[%d]=%d\n",i,alternatives[i]);
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
	printf("\n sending type: %d\n",work.type);
	printf("\n sending alternatives: %d\n",work.num_alternatives);


    MPI_Send(&work, 1, work_mpi_datatype, num_process, tag_work, MPI_COMM_WORLD);
    MPI_Type_free( &work_mpi_datatype);
	//sending dinamic arrays
	MPI_Send( &values, num_values, MPI_DOUBLE, num_process, tag_values, MPI_COMM_WORLD );
	MPI_Send( &serialized_tasks, len, MPI_CHAR, num_process, tag_tasks, MPI_COMM_WORLD );
	MPI_Send( &serialized_resources, len_resources, MPI_CHAR, num_process, tag_resources, MPI_COMM_WORLD );

	if(num_alternatives>0)
    {
    	MPI_Send( &alternatives_to_work, num_alternatives, MPI_INT, num_process, tag_alternatives, MPI_COMM_WORLD );
    }
	MPE_Log_event(event1b, 0, "end send");
	return res;
}

int send_resolved(const PalgorithmPD palg)
{
	MPE_Log_event(event5a, 0, "start send resolved");
	int res;
	int num_process=0;//to master
	MPI_Request request;
	MPI_Status  status;

	printf("\nSENDING RESOLVED TO MASTER");
	struct Resolved resolved;
	int num_resolved=palg->num_solved;
	if(num_resolved==0)
	{
		printf("\n SENDING NO SOLUTION");

	}
	else
	{
		for(int i=0;i<num_resolved;i++){
			resolved.value[i]=palg->solvedProblems[i].solution.acum;//TODO
			for(int j=0;j<palg->problems->aproblem.numResources;j++){
				resolved.resources[i+j]=palg->solvedProblems[i].solution.resources->position;
			}

		}

	}
	resolved.num_resolved=num_resolved;
	//num_resolved=1;//TODO test
	printf("\nSENDING RESOLVED TO MASTER2");
	MPI_Datatype resolved_mpi_datatype;
	int blocklengths[3] = {1,1,1};
	MPI_Datatype types[3] = {MPI_DOUBLE,MPI_INT,MPI_DOUBLE};
	const MPI_Aint offsets[3]= { 0, sizeof(double),sizeof(int)*100+sizeof(double)};

	MPI_Type_create_struct(3, blocklengths, offsets, types,  &resolved_mpi_datatype);
	MPI_Type_commit ( &resolved_mpi_datatype);-
	printf("\n+++++++++++++++++++++++++++++++++++++++++++\npre send mpi");
	MPE_Init_log();
//	MPE_Log_event(1, 0, "start broadcast");
	MPI_Send( &resolved, 1, resolved_mpi_datatype, num_process, tag_resolved, MPI_COMM_WORLD );
//	MPE_Log_event(2, 0, "end broadcast");
	printf("\n+++++++++++++++++++++++++++++++++++++++++++\npost send mpi %f",resolved.value);
	MPI_Type_free( &resolved_mpi_datatype);
	MPE_Log_event(event5b, 0, "end send resolved");
	return 0;
}
int rcv_best()
{
	double new_best;
	MPI_Request request;
	MPE_Log_event(event4a, 0, "start receive best");
	MPI_Irecv(&new_best,1,MPI_DOUBLE,MPI_ANY_SOURCE,tag_best,MPI_COMM_WORLD,&request);
	printf("===========================\n"
			"============================\n"
			"RCV best: %f", new_best);
	MPE_Log_event(event4b, 0, "end receive best");
	return 0;
}
int send_best(PalgorithmPD palg)
{
	int res=0;
	double best=palg->best;
	int count = 1;
	MPE_Log_event(event3a, 0, "start send best");
	MPI_Request request = MPI_REQUEST_NULL;
	printf("\n sending best: %f to %d",best,master);
	MPI_Isend(&best, count, MPI_INT, master, tag_best, MPI_COMM_WORLD, &request);
	printf("\n send best: %f to %d",best,master);
	MPE_Log_event(event3b, 0, "end send best");
	return res;
}
int broadcast_best(PalgorithmPD palg){
	int res=0;
	double best;
	int count = 1;
	int rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank==master){
		best=palg->best;
	}
	MPI_Bcast(&best, count, MPI_DOUBLE, master, MPI_COMM_WORLD);
	return res;
}

void waitting_best(PalgorithmPD palg)
{
	double best=SMALL;//TODO for diferent max min
	int ready;

	MPI_Request found_request;
	MPI_Irecv(&best, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag_best, MPI_COMM_WORLD, &found_request);//TODO provisional
	MPI_Test(&found_request, &ready, MPI_STATUS_IGNORE);
	if(ready)
	{
		printf("\nSomeone has found a better result");
	}
	else
	{
		printf("\nNOT found better result");
	}
}

int scan_petition(MPI_Request *request_petition)
{
	MPE_Log_event(event2, 0, "listen");
	MPI_Status status;
	int n=0;
	int flag=0;
	int sender;
	printf("\n 				PRE TEST");
	MPI_Test(request_petition, &flag, &status);
	printf("\n 				POST TEST");
	if(flag)
	{
		sender=status.MPI_SOURCE;
		printf("\n 				SENDER %d", sender);
		MPE_Log_event(event3, 0, "rcve petition");

		give_me_work(1);//TODO
		MPI_Irecv(&n, 1, MPI_INT, MPI_ANY_SOURCE, tag_ask_work, MPI_COMM_WORLD, request_petition);

	}

	return n;
}

int give_me_work(int process)
{
	int n;
	MPI_Request request_w;
	MPI_Request request_rcv;
	MPI_Datatype node_mpi_datatype;
	int blocklengths[3] = {1,100,1};
	MPI_Datatype types[3] = {MPI_INT, MPI_INT,MPI_DOUBLE};
	const MPI_Aint offsets[3]= { 0, sizeof(int),sizeof(int)*101};
	MPI_Type_create_struct(3, blocklengths, offsets, types,  &node_mpi_datatype);
	MPI_Type_commit ( &node_mpi_datatype );
	MPI_Status status;
	MPI_Isend(&n, 1, MPI_INT, process, tag_give_work, MPI_COMM_WORLD, &request_w);
	struct  Node_work node_w;
	MPI_Irecv(&node_w, 1, node_mpi_datatype, process, tag_ask_work, MPI_COMM_WORLD, &request_rcv);

	return 0;

}

int init_listening(MPI_Request *request_petition)
{
	int n;
	MPI_Irecv(&n, 1, MPI_INT, MPI_ANY_SOURCE, tag_ask_work, MPI_COMM_WORLD, request_petition);
	return 0;
}

int finish_work()
{
	rcv_resolved();
	//TODO
	printf("\n END OF FINISH WORK ON MASTER\n ");
	printf("\n BEST: %f\n",final_alg.best);
	return 0;
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
		printf("\n*************************************************************************"
				"name resource %d in serializer: %s",i,palg->ppd.aproblem.resources[i].name);
		strcat(temp,palg->ppd.aproblem.resources[i].name);
		strcat(temp,divisor);
	}
	strcpy(all,temp);
	printf("\ncopied RESOURCES---->>>>%s\n",all);
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
			  printf("\nDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD_Distribition_PD");
			  printf("\nproblem %d of %d problems in alg",w,lengthNewArrayAppd);
			  show_aproblem_PD(&(palg->problems[w]));
		  }
		  palg->num_problems=lengthNewArrayAppd;
		  printf("\n::finish pd distribution");
  return res;
}
