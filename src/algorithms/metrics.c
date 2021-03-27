/*
 * metrics.c
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */

#include "metrics.h"

int init_clock()
{
	startwtime = MPI_Wtime();
	return 0;
}

double end_clock()
{
	  endwtime = MPI_Wtime();
	  double res=endwtime-startwtime;
	  printf("clock time = %f\n", res);
	  return res;
}

int describe_logs(){


	  /*  Get event ID from MPE, user should NOT assign event ID  */
    MPE_Describe_state(event1a, event1b, "Send work", "red");
    MPE_Describe_state(event2a, event2b, "Receive work",   "blue");
    MPE_Describe_state(event3a, event3b, "Send best",    "green");
    MPE_Describe_state(event4a, event4b, "Receive best",      "orange");

    MPE_Describe_event( event1, "Broadcast", "white" );
    MPE_Describe_event( event2, "Compute Start", "purple" );

    return 0;
}
