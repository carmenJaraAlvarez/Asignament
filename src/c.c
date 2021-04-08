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
#include "algorithms/metrics.h"

int ierr;
int master=0;
int numprocs=0;
int print_all=0;//to help to debug. Simple logs for developer

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

int event1, event2, event3, event4, event5, event6, event7;
int startEvent, endEvent;

double startwtime, endwtime;

extern int init_slaves;
extern MPI_Request request;
MPI_Request request_b=MPI_REQUEST_NULL;
double best;
int main(int argc, char **argv)
{
  int myid;



  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);



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

  MPE_Log_get_solo_eventID( &event1 );
  MPE_Log_get_solo_eventID( &event2 );
  MPE_Log_get_solo_eventID( &event3 );
  MPE_Log_get_solo_eventID( &event4 );
  MPE_Log_get_solo_eventID( &event5 );
  MPE_Log_get_solo_eventID( &event6 );
  MPE_Log_get_solo_eventID( &event7 );

  init_slaves=0;

  //MPE_Log_sync_clocks();

  if (myid != 0)  {
	  ////////////////////

	  MPI_Ibcast(&best,1,MPI_DOUBLE,0,MPI_COMM_WORLD, &request_b);

	  ////////////////////

	  MPI_Bcast(&init_slaves,1,MPI_INT,0,MPI_COMM_WORLD);//waiting master's order
	  rcv_work();
	  MPI_Ibarrier(MPI_COMM_WORLD, &request);//to know every process is finished
  }
  else//master
  {
	  init_best(&request_b);
	describe_logs();
	if(print_all)
	{
		printf("\nAn MPI program.\n The number of processes is %d \n This is %d\n",numprocs,myid);
	}

	GtkWidget *window_init;
	gtk_init(&argc, &argv);

	//the particular problem/////////////
    // PAproblem pap;
	create_aproblem_window(&window_init,numprocs);
	/////////////////////////////////////

	gtk_main();

  }
  int rcv_bcast=0;
  MPI_Status status;
  if(1)
  {
	  while(!rcv_bcast)
	   {
	 	  printf( "\ntesting");
	 	  MPI_Test(&request_b,&rcv_bcast,&status);
	   }
	   if(rcv_bcast)
	   {
	 	  printf( "\nMessage from process %d : best %f\n", myid, best);
	   }
	   waiting_best(&request);//testing function
  }

  //MPE_Log_sync_clocks();

  MPE_Finish_log("c");
  MPI_Finalize();
}
