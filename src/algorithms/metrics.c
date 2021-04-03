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
    MPE_Describe_state(event5a, event5b, "Send resolved",    "white");
    MPE_Describe_state(event6a, event6b, "Receive resolved",      "purple");

    MPE_Describe_event( event1, "Ask for work", "white" );
    MPE_Describe_event( event2, "Scan petition", "orange" );
    MPE_Describe_event( event3, "Receive petition", "blue" );
    MPE_Describe_event( event4, "Receive best", "green" );
    MPE_Describe_event( event5, "Broadcast", "pink" );
    MPE_Describe_event( event6, "prune", "purple" );
    MPE_Describe_event( event7, "...", "pink" );

    return 0;
}
