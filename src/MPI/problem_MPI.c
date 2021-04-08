
#include "problem_MPI.h"
#include <stddef.h>

static int pD_distribution(PalgorithmPD palg);


//we will free process 0 from problem resolution.
int distribution(PalgorithmPD palg)
{
	int res=-1;
	Solution sol;
	int num_slaves=numprocs-1;
    MPI_Request request;
    MPI_Status status;
    int flag;

    if(print_all)
    {
    	printf("\n num problems-num slaves: %d-%d",palg->num_problems,num_slaves);
    }

	do
	{
		if(print_all)
		{
			printf("\n num subpr: %d",get_num_subproblems());
		}

		for(int i=0;i<get_num_subproblems();i++)
		{
			pD_distribution(palg);
		}
	}while(palg->isRandomize && get_PDsolution(palg,&sol)!=0);


	if(palg->num_problems==num_slaves)
	{
		if(print_all)
		{
			printf("\nif");
		}


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
		if(print_all)
		{
			printf("\n else if");
		}

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
			if(print_all)
			{
				printf("\n0000000000000000000000000000000000000000000000000000sending case num problems >num processes. resource 0: %s",palg->ppd.aproblem.resources[0].name);

			}
			int alternatives[100];//TODO
			for(int j=0;j<rounds+1;j++)
			{
				if(print_all)
				{
					printf("\n1111111111111111111111111111111111111111111111111111sending case num problems >num processes. resource 0: %s",palg->ppd.aproblem.resources[0].name);
				}

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
	if(print_all)
	{
		printf("00000000000000000000000000 outgoing distribution");
	}

	return res;


}

int rcv_work()
{
	MPE_Log_event(event2a, 0, "start receive work");
	int res=0;
	int ierr=0;
	if(print_all)
	{
		printf("rcv...\n");
	}

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

    if(print_all)
    {
        printf("\n received num task: %d",work.num_tasks);
    	printf("\n received num resources: %d\n",work.num_resources);
    }

	int size_values=work.num_tasks*work.num_resources;


	//getting values

	double values[size_values];

	MPI_Recv ( &values, size_values, MPI_DOUBLE, MPI_ANY_SOURCE, tag_values, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	if(print_all)
	{
		printf("%d resources\n",work.num_resources);
		printf("%d received size values\n",size_values);
		printf("%d received type\n",work.type);
		printf("%f received first\n",values[0]);
		printf("%f received second\n",values[1]);
	}


	//getting tasks and resources names

	char serialized[1000];//TODO
	Task tasks[work.num_tasks];

	char serialized_resources[1000];//TODO
	Resource resources[work.num_resources];

	MPI_Recv( &serialized, 1000, MPI_CHAR, MPI_ANY_SOURCE, tag_tasks, MPI_COMM_WORLD,MPI_STATUS_IGNORE );

	deserializer_tasks(&serialized,work.num_tasks,tasks);
	if(print_all)
	{
		printf("*************************POST RCVE TASKS name 3º task\n%s\n",tasks[2].name);
	}

	MPI_Recv( &serialized_resources, 1000, MPI_CHAR, MPI_ANY_SOURCE, tag_resources, MPI_COMM_WORLD,MPI_STATUS_IGNORE );
	deserializer_resources(&serialized_resources,work.num_resources,resources);
	if(print_all)
	{
		printf("*************************POST RCVE RESOURCES name 3º resource\n%s\n",resources[2].name);

	}

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
	//printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	show_aproblem(&a);

    init_work(&a, work.num_alternatives, &alternatives);//TODO testing mpe
    printf("\n outgoing rcv_work");


////    printf("\n Inside RCV. num alternatives: %d", work.num_alternatives);
////    for(int a=0;a<work.num_alternatives;a++)
////    {
////    	printf("\n alternative %d=%d", a,alternatives[a]);
////    }
    if(print_all)
    {
    	printf("+\n++++++++++++++++++++++++++++END RCV_work");

    }

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
	if(print_all)
	{
		printf("+\n+++++++++++++++++++++++++++++++++++++++++++++++++++\nINSIDE RCV_REVOLVED\n");

	}
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
		if(print_all)
		{
			printf("+\n+++++++++++++++++++++++++++++++++++++++++++++++++++"
					"\npre mpi_rcev\n");
		}


		MPI_Recv(&resolved, 1, resolved_mpi_datatype , p, tag_resolved, MPI_COMM_WORLD, &status);

		if(resolved.num_resolved>0)//TODO
		{
			//palg->best=resolved.value;
			if(print_all)
			{
				printf("\n ONE BEST IN MASTER-> %f",resolved.value[0]);
			}

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
		if(print_all)
		{
			printf("+\n+++++++++++++++++++++++++++++++++++++++++++++++++++\npost mpi_rcev\n"
					"******************received resolved from %d\n ",p);
		}

	}



    MPI_Type_free ( &resolved_mpi_datatype);
    MPE_Log_event(event6b, 0, "end receive resolved");
    if(print_all)
    {
    	  printf("+\n+++++++++++++++++++++++++++++++++++++++++++++++++++\nEND RCV RESOLVED\n");

    }
    return res;

}

int init_work(PAproblem pa, int num_alternatives, int * alternatives)
{
	int res=0;
	AproblemPD appd;
	initAProblemPD(&appd, pa);
	AlgorithmPD alg;
	init_algorithmPD(&alg, appd);
	if(print_all)
	{
		printf("best post init: %f",alg.best);
		show_aproblem(&(alg.ppd.aproblem));
	}

	new_best=alg.best;
	MPE_Log_event(event5, 0, "init listening broadcast");
	MPI_Irecv(&new_best, 1, MPI_DOUBLE, master, tag_best, MPI_COMM_WORLD, &request_bcast);
	if (num_alternatives==0){
		ask_work();
		if(print_all)
		{
			printf("\n-----------------------------------------------\n");

		}
		alg.num_solved=0;
		send_resolved(&alg);
	}
	else
	{
		if(num_alternatives==1)
		{
			if(print_all)
			{
				printf("\n	===========only 1 alternative");
			}

			alg.ppd.index=1;
			strcpy(alg.ppd.solution.resources[alg.ppd.solution.lengthArrays].name, pa->resources[alternatives[0]].name);
			alg.ppd.solution.resources[alg.ppd.solution.lengthArrays].position=alternatives[0];

			alg.ppd.solution.lengthArrays=alg.ppd.solution.lengthArrays+1;
			alg.ppd.solution.acum=pa->values[0+alternatives[0]*alg.ppd.aproblem.numTask];
			alg.problems[0]=alg.ppd;if(print_all)
			{
				printf("Assert index =1 in problems post work:%d ",alg.problems[0].index);
			}

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
				if(print_all)
				{
					printf("\n	=============== Alternative: %d\n",alt[i].indexResource);
					printf("\n PPD index: %d\n",alg.ppd.index);
				}

				get_subproblem(&alg.ppd, &subproblems[i],alt[i], get_num_subproblems());
				if(print_all)
				{
					printf("\n "
							"========================Inside init work more than 1 alternative");
					show_aproblem_PD(&subproblems[i]);
					printf("\n post get subproblems index: %d",subproblems[i].index);
					printf("\n post get subproblems solution.len: %d",subproblems[i].solution.lengthArrays);
					printf("\n post get subproblems solution.acum: %f",subproblems[i].solution.acum);
					printf("\n post get subproblems first resource: %s",subproblems[i].solution.resources[0].name);
				}

				alg.problems[i]=subproblems[i];
				if(print_all)
				{
					printf("\n post get subproblems index: %d",subproblems[i].index);
					printf("\n post get subproblems solution.len: %d",alg.problems[i].solution.lengthArrays);
					printf("\n post get subproblems solution.acum: %f",alg.problems[i].solution.acum);
					printf("\n post get subproblems first resource: %s",alg.problems[i].solution.resources[0].name);

				}

				if(i>0)
				{
					printf("\nProblem i-1:\n");
					show_aproblem_PD(&(alg.problems[i-1]));
				}

				strcpy(alg.problems[i].solution.resources[alg.problems[i].solution.lengthArrays].name, pa->resources[alternatives[i]].name);
				alg.problems[i].solution.resources[alg.problems[i].solution.lengthArrays].position=pa->resources[alternatives[i]].position;
				if(print_all)
				{
					printf("\nlen solution %d ccccccccccccccccccccccccccccc",alg.problems[i].solution.lengthArrays);
					printf("\nacum solution %f ccccccccccccccccccccccccccccc",alg.problems[i].solution.acum);
					printf("Assert index =1 in problems post work:%d \n",alg.problems[i].index);

				}

			}
			alg.ppd.index=1;
			alg.num_problems=num_alternatives;//TODO
		}
		for(int i=0;i<alg.num_problems;i++){
			show_aproblem_PD(&(alg.problems[i]));
		}
		if(print_all)
		{
			printf("\nBefore exec algoritm\n");
		}

		exec_algorithm(&alg);
		if(print_all)
		{
			printf("\nAfter exec algoritm\n");
		}
		Solution sol;
		get_PDsolution(&alg, &sol);
		for(int i=0;i<alg.ppd.solution.lengthArrays;i++){
			printf("\nResources: \n*%s\n",alg.ppd.solution.resources[i].name);
		}
		if(print_all)
		{
			printf("Solution value: %f", alg.ppd.solution.acum);
		}

		send_resolved(&alg);
		if(print_all)
		{
			printf("\nSEND RESOLVED TO MASTER");
		}

		delete_algorithmPD(&alg);
	}//end else (alternative>0)
	if(print_all)
	{
		printf("\noutgoing initwork");
	}

	return res;
}
int send_work(const PalgorithmPD palg,int *alternatives, int num_alternatives, int num_process)
{
	MPE_Log_event(event1a, 0, "start send");
	if(print_all)
	{
		printf("\n...................sending  %d  alternatives to process %d\n",num_alternatives,num_process);
	}
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
	if(print_all)
	{
		printf("\n In work task: %d",work.num_tasks);
		printf("\n In work resources: %d",work.num_resources);
	}


	double values[num_values];
	int alternatives_to_work[num_alternatives];
	Cadena tasks[num_values];
	Cadena resources[num_values];

	for(int i=0;i<num_values;i++)
	{
		values[i]=palg->ppd.aproblem.values[i];
		if(print_all)
		{
			printf("inside send. value[%d]=%f\n",i,values[i]);
		}

	}
	for(int i=0;i<num_alternatives;i++)
	{
		alternatives_to_work[i]=alternatives[i];//TODO
		if(print_all)
		{
			printf("inside send. alternative[%d]=%d\n",i,alternatives[i]);
		}

	}
	for(int i=0;i<work.num_resources;i++)
	{
		strcpy(resources[i],palg->ppd.aproblem.resources[i].name);
		if(print_all)
		{
			printf("inside send. resources[%d]=%s\n",i,resources[i]);
		}

	}
	for(int i=0;i<work.num_tasks;i++)
	{
		strcpy(tasks[i],palg->ppd.aproblem.tasks[i].name);
		if(print_all)
		{
			printf("inside send. resources[%d]=%s\n",i,tasks[i]);
		}

	}

	//sending
	MPI_Datatype work_mpi_datatype;
	int blocklengths[4] = {1,1,1,1};
	MPI_Datatype types[4] = {MPI_INT, MPI_INT,MPI_INT, MPI_INT};
    const MPI_Aint offsets[4]= { 0, sizeof(int),sizeof(int)*2, sizeof(int)*3};
	MPI_Type_create_struct(4, blocklengths, offsets, types,  &work_mpi_datatype);
	MPI_Type_commit ( &work_mpi_datatype);

	if(print_all)
	{
		printf("\n sending num task: %d",work.num_tasks);
		printf("\n sending num resources: %d\n",work.num_resources);
		printf("\n sending type: %d\n",work.type);
		printf("\n sending alternatives: %d\n",work.num_alternatives);
	}

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
	if(print_all)
	{
		printf("\nSENDING RESOLVED TO MASTER");
	}


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
	if(print_all)
	{
		printf("\nSENDING RESOLVED TO MASTER2");
	}

	MPI_Datatype resolved_mpi_datatype;
	int blocklengths[3] = {1,1,1};
	MPI_Datatype types[3] = {MPI_DOUBLE,MPI_INT,MPI_DOUBLE};
	const MPI_Aint offsets[3]= { 0, sizeof(double),sizeof(int)*100+sizeof(double)};

	MPI_Type_create_struct(3, blocklengths, offsets, types,  &resolved_mpi_datatype);
	MPI_Type_commit ( &resolved_mpi_datatype);
	if(print_all)
	{
		printf("\n+++++++++++++++++++++++++++++++++++++++++++\npre send mpi");
	}

	MPE_Init_log();

	MPI_Send( &resolved, 1, resolved_mpi_datatype, num_process, tag_resolved, MPI_COMM_WORLD );
	if(print_all)
	{
		printf("\n+++++++++++++++++++++++++++++++++++++++++++\npost send mpi %f",resolved.value);
	}

	MPI_Type_free( &resolved_mpi_datatype);
	MPE_Log_event(event5b, 0, "end send resolved");
	return 0;
}
int rcv_best(double rcvd_best, MPI_Request *request_bcast)
{
	if((rcvd_best>final_alg.best && final_alg.ppd.aproblem.type==MAX) ||
			(rcvd_best<final_alg.best && final_alg.ppd.aproblem.type==MIN)){
		final_alg.best=rcvd_best;
		MPE_Log_event(event5, 0, "start broadcast");
		broadcast_best(rcvd_best);
		if(print_all)
		{
			printf("\n INSIDE RCV BEST AND IS BETTER");
		}

	}
	return 0;
}
int send_best(PalgorithmPD palg)
{
	int res=0;
	double best=palg->best;
	int count = 1;
	MPE_Log_event(event3a, 0, "start send best");
	MPI_Request request_b;
	if(print_all)
	{
		printf("\n sending best: %f to %d",best,master);
	}

	MPI_Isend(&best, count, MPI_DOUBLE, master, tag_best, MPI_COMM_WORLD, &request_b);
	if(print_all)
	{
		printf("\n send best: %f to %d",best,master);
	}

	MPE_Log_event(event3b, 0, "end send best");
	return res;
}
int broadcast_best(double better){
	int res=0;
	double best_to_broadcast;
	int count = 1;
	int rank;
	MPE_Log_event(event5, 0, "broadcast");
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank==master){
		best_to_broadcast=better;
		if(print_all){
			printf("\n I am %d, and the best to broadcast is %f",rank, best_to_broadcast);
		}

		MPE_Log_event(event5, 0, "broadcast best");
		MPI_Ibcast(&best_to_broadcast, count, MPI_DOUBLE, master, MPI_COMM_WORLD, &request_bcast);
	}

	return res;
}

void waiting_best(MPI_Request* request_b)
{
	int ready=0;

	MPI_Test(request_b, &ready, MPI_STATUS_IGNORE);
	if(ready)
	{
		if(1)
		{
			printf("\nMaster has send best result %f", best);
		}

		MPE_Log_event(event5, 0, "broadcast best");
//		MPI_Irecv(&best, 1, MPI_DOUBLE, master, tag_best, MPI_COMM_WORLD, &request_bcast);
//		if((best>palg->best && palg->ppd.aproblem.type==MAX) ||
//				(best<palg->best && palg->ppd.aproblem.type==MIN)){
//			palg->best=best;
//		}
//		if((best>final_alg.best && palg->ppd.aproblem.type==MAX) ||
//				(best>final_alg.best && palg->ppd.aproblem.type==MIN)){
//			final_alg.best=best;
//		}
	}
}
int log_prune()
{

	return 0;
}

int scan_petition(MPI_Request *request_ask_work, MPI_Request *request_best, MPI_Request *request_bcast)
{
	MPE_Log_event(event2, 0, "listen");
	MPI_Status status;
	MPI_Status status_best;
	int flag=0;
	int flag_b=0;
	int sender;
//	printf("\n 				PRE TEST");
	MPI_Test(request_best, &flag_b, &status_best);
	if(flag_b)
	{
		sender=status_best.MPI_SOURCE;
		if(print_all)
		{
			printf("\n 				SENDER BEST %d\n"
					"				Best %f", sender, b);
		}

		MPE_Log_event(event4, 0, "rcve best");
		double new_best=b;
		rcv_best(new_best, request_bcast);
		MPI_Irecv(&b, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag_best, MPI_COMM_WORLD, request_best);
		flag_b=0;
	}
	MPI_Test(request_ask_work, &flag, &status);
	if(flag)
	{
		sender=status.MPI_SOURCE;
		if(print_all)
		{
			printf("\n 				SENDER PETITION %d\n"
					"				PETITION %d", sender, n);
		}

		MPE_Log_event(event3, 0, "rcve petition");

		give_me_work(1);//TODO
		MPI_Irecv(&n, 1, MPI_INT, MPI_ANY_SOURCE, tag_ask_work, MPI_COMM_WORLD, request_ask_work);

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
void init_best(MPI_Request * request_b){
	  ////////////////////
	  best=final_alg.best;

	  if(1)
	  {
		 printf("\n init best()-------------------%f",final_alg.best);
	  }
	  MPI_Ibcast(&best,1,MPI_DOUBLE,0,MPI_COMM_WORLD, &request_b);
}

int init_listening(MPI_Request *request_petition,MPI_Request *request_best )
{

	MPI_Irecv(&n, 1, MPI_INT, MPI_ANY_SOURCE, tag_ask_work, MPI_COMM_WORLD, request_petition);
	MPI_Irecv(&b, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag_best, MPI_COMM_WORLD, request_best);
	return 0;
}

int finish_work()
{
	rcv_resolved();
	//TODO
	if(print_all)
	{
		printf("\n END OF FINISH WORK ON MASTER\n ");
		printf("\n BEST: %f\n",final_alg.best);
	}

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
		if(print_all)
		{
			printf("\n----%s\n",temp);
		}

	}
	strcpy(all,temp);

	int len;
	for (len = 0; all[len] != '\0'; ++len);
	if(print_all)
	{
		printf("\n---->>>>%s\n",all);
		printf("Length of Str tasks is %d", len);
	}


	return len;
}
int deserializer_tasks(char* all, int len, PTask tasks)
{

	Cadena aux="";
	char divisor=';';
	if(print_all)
	{
		printf("\nINSIDE DESERIALIZED---->>>>%s\n",all);
	}

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
				if(print_all)
				{
					printf("\nfound %d *\n",count);
				}

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
		if(print_all)
		{
			printf("\n*************************************************************************"
							"name resource %d in serializer: %s",i,palg->ppd.aproblem.resources[i].name);

		}
		strcat(temp,palg->ppd.aproblem.resources[i].name);
		strcat(temp,divisor);
	}
	strcpy(all,temp);
	int len;
	for (len = 0; all[len] != '\0'; ++len);
	if(print_all)
	{
		printf("\ncopied RESOURCES---->>>>%s\n",all);
		printf("Length of Str resources is %d", len);
	}

	return len;
}
int deserializer_resources(char* all, int len, PResource resources)
{

	Cadena aux="";
	char divisor=';';
	if(print_all)
	{
		printf("\nINSIDE DESERIALIZED  resources---->>>>%s\n",all);
	}

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
				if(print_all)
				{
					printf("\nfound resources %d *\n",count);
				}

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
				  if(print_all)
				  {
					  printf("        Alternatives: ");
					  for(int k=0;k<numAlternatives;k++)
					  {
						  printf("%d ", as[k].indexResource);
					  }
					  printf("\n");

				  }

				  randomize(palg,as);//not using
				  Logico ismin;
				  Logico ismax;
				  for(int u=0;u<numAlternatives;u++)
				  {
					  //prune
					  ismin=is_min(palg);
					  ismax=is_max(palg);
					  double b_estimated=get_best_estimate(&problems[0]);
					  if((ismin && b_estimated<=palg->best)
										  || (ismax && b_estimated>=palg->best))
					  {
						  //case no prune,control our worst is better than global to change it
						  double w_estimated=get_worst_estimate(&problems[0]);
						  if((w_estimated>final_alg.best && ismax) ||
								  (w_estimated<final_alg.best && ismin)  )
						  {
							  final_alg.best=w_estimated;
						  }
						  ////////////////////////////////////////////////////////////////////
						  int numSubproblems=get_num_subproblems();
						  AproblemPD appdNew;
						  for(int j=0;j<numSubproblems;j++)
						  {
							  initAProblemPD(&appdNew,&(palg->ppd.aproblem));

							  get_subproblem(&problems[0], &appdNew, as[u],numSubproblems);
							  if(print_all)
							  {
								  printf("\n     is NOT base case: last appdNew sol: %s\n",appdNew.solution.resources[appdNew.solution.lengthArrays-1].name);
								  printf("     i=%d of %d alternatives\n",u, numAlternatives);

							  }
							   //if problem//TODO
									  //add problem to new array
							  copy_aproblem_PD( &(newArrayAppd[lengthNewArrayAppd]),appdNew);
							 lengthNewArrayAppd++;

						  }//end for num subproblem=1
					  }//end if not prune
					  else
					  {
						  MPE_Log_event(event6, 0, "prune in distribution");
					  }
				   }//end for alternatives
			  }//end else (not base case)
			}//end else (exits alternative)

		  for(int w=0; w<lengthNewArrayAppd;w++)
		  {
			  palg->problems[w]=newArrayAppd[w];
			  if(print_all)
			  {
				  printf("\nDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD_Distribition_PD");
				  printf("\nproblem %d of %d problems in alg",w,lengthNewArrayAppd);
			  }

			  show_aproblem_PD(&(palg->problems[w]));
		  }
		  palg->num_problems=lengthNewArrayAppd;
		  if(print_all)
		  {
			  printf("\n::finish pd distribution");
		  }

  return res;
}
