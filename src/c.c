/*
 * c.c
 *
 *  Created on: 7 mar. 2021
 *      Author: practica
 */


/* */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include "mpe.h"
#include "mpe_log.h"
#include "common/logico.h"
#include "assignment/aproblem.h"
#include "assignment/aproblem_gui.h"
#include "../tests/test_resource.c"
#include "../tests/test_task.c"
#include "../tests/test_aproblem.c"
#include "../tests/test_aproblemPD.c"
#include "../tests/test_aPD_algorithm.c"
#include "../tests/test_application.c"
#include "algorithms/metrics.h"


int master=0;
int numprocs=0;
int print_all=0;//to help to debug. Simple logs for developer
//to launch tests///////
int tests=0;
Cadena test;
int var_test[7];
///////////////////////


int event1a;
int event1b;
int event2a;
int event2b;
int event3a;
int event3b;
int event4a;
int event4b;
int event5a;
int event5b;
int event6a;
int event6b;
int event7a;
int event7b;
int event8a;
int event8b;
int event9a;
int event9b;

int event1, event2, event3, event4, event5, event6, event7, event8, event9;
int startEvent, endEvent;

double startwtime, endwtime;
MPI_Comm world;
extern int init_slaves;
extern MPI_Request request;
MPI_Request request_b=MPI_REQUEST_NULL;


double best;
int main(int argc, char **argv)
{
	if(print_all)
	{
		for(int i=0;i<argc;i++)
		{
			printf("\nARG %d->%s",i, argv[i]);
		}
	}


	if(argc>8 && strcmp(argv[1],"test")==0)
	{
		tests=1;
		strcpy(test,argv[2]);
		if(print_all)
		{
			printf("\n test %s",test);
		}
		for(int i=0;i<7;i++)
		{
			var_test[i]=atoi(argv[i+2]);
			if(print_all)
			{
				printf("\nc.c	main()	arg %d",var_test[i]);
			}
		}
	}


  int myid;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  MPI_Request request_work[numprocs-1];


  MPI_Comm_dup(MPI_COMM_WORLD,&world);
  MPE_Init_log();

  /*  Get event ID from MPE, user should NOT assign event ID  */
  MPE_Log_get_state_eventIDs( &event1a, &event1b );
  MPE_Log_get_state_eventIDs( &event2a, &event2b );
  MPE_Log_get_state_eventIDs( &event3a, &event3b );
  MPE_Log_get_state_eventIDs( &event4a, &event4b );
  MPE_Log_get_state_eventIDs( &event5a, &event5b );
  MPE_Log_get_state_eventIDs( &event6a, &event6b );
  MPE_Log_get_state_eventIDs( &event7a, &event7b );
  MPE_Log_get_state_eventIDs( &event8a, &event8b );
  MPE_Log_get_state_eventIDs( &event9a, &event9b );

  MPE_Log_get_solo_eventID( &event1 );
  MPE_Log_get_solo_eventID( &event2 );
  MPE_Log_get_solo_eventID( &event3 );
  MPE_Log_get_solo_eventID( &event4 );
  MPE_Log_get_solo_eventID( &event5 );
  MPE_Log_get_solo_eventID( &event6 );
  MPE_Log_get_solo_eventID( &event7 );
  MPE_Log_get_solo_eventID( &event8 );
  MPE_Log_get_solo_eventID( &event9 );
  init_slaves=0;

  //MPE_Log_sync_clocks();
  double buffer=1.;
  int buffer_work=0;
  if (myid != 0)  {
	  ////////////////////

	  //MPI_Ibcast(&buffer,1,MPI_DOUBLE,0,world, &request_b);

	  //init_waiting_best(&buffer,&request_b);
	  ////////////////////

	  MPI_Bcast(&init_slaves,1,MPI_INT,0,MPI_COMM_WORLD);//waiting master's order
	  rcv_work(&buffer,&request_b,&buffer_work);
		if(print_all)
		{
			printf("\nThis is %d previous ibarrier\n",myid);
		}
	  MPI_Ibarrier(MPI_COMM_WORLD, &request);//to know every process is finished
		if(print_all)
		{
			printf("\nThis is %d post ibarrier\n",myid);
		}
  }
  else//master
  {

	init_best(&request_b,&world,numprocs);

	describe_logs();
	if(print_all)
	{
		printf("\nAn MPI program.\n The number of processes is %d \n This is %d\n",numprocs,myid);
	}


	gtk_init(&argc, &argv);

	//the particular problem/////////////
    // PAproblem pap;
	create_aproblem_window(numprocs);
	/////////////////////////////////////
	if(!tests)
	{
		if(print_all)
		{
			printf("\nc.c	main()	No test");
		}
		gtk_main();
	}


  }

  //MPE_Log_sync_clocks();

  MPE_Finish_log("c");
  MPI_Finalize();
}
