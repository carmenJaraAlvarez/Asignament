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
int numprocs;

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

int event1, event2, event3;
int startEvent, endEvent;

double startwtime, endwtime;

int main(int argc, char **argv)
{
  int n, myid,all_finished=0;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  MPI_Request request;
  MPI_Request request_petition;

  //init_logs();

  MPE_Init_log();

  /*  Get event ID from MPE, user should NOT assign event ID  */
  MPE_Log_get_state_eventIDs( &event1a, &event1b );
  MPE_Log_get_state_eventIDs( &event2a, &event2b );
  MPE_Log_get_state_eventIDs( &event3a, &event3b );
  MPE_Log_get_state_eventIDs( &event4a, &event4b );
  MPE_Log_get_state_eventIDs( &event5a, &event5b );
  MPE_Log_get_state_eventIDs( &event6a, &event6b );

  MPE_Log_get_solo_eventID( &event1 );
  MPE_Log_get_solo_eventID( &event2 );
  MPE_Log_get_solo_eventID( &event3 );

  if (myid == 0)
  {
	describe_logs();

    printf("\nAn MPI program.\n The number of processes is %d \n This is %d\n",numprocs,myid);

	GtkWidget *window;
	gtk_init(&argc, &argv);

	//the particular problem/////////////
    // PAproblem pap;
	create_aproblem_window(&window,numprocs);
	/////////////////////////////////////

	gtk_main();

  }

  MPE_Log_sync_clocks();
  if (myid != 0)  {
	  rcv_work();

  }
  MPI_Ibarrier(MPI_COMM_WORLD, &request);

  //printf("[MPI process %d] I got my rank, it is %d, I now call MPI_Ibarrier.\n", myid, myid);
  if (myid == 0)
  {
	  init_listening(&request_petition);
	  while(!all_finished)
	  {
		  scan_petition(&request_petition);
		  MPI_Test(&request,&all_finished, MPI_STATUS_IGNORE);//test barrier
	  }
	  finish_work();
//	  rcv_resolved();//TODO TEST
	  end_clock();
  }
  MPE_Finish_log("c");
  MPI_Finalize();
}
