/*
 ============================================================================
 Name        : c.c
 Author      : cj

 ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

#include "common/logico.h"
#include "assignment/aproblem.h"
#include "assignment/aproblem_gui.h"
#include "../tests/test_resource.c"
#include "../tests/test_task.c"
#include "../tests/test_aproblem.c"
#include "../tests/test_aproblemPD.c"
#include "../tests/test_aPD_algorithm.c"


int main(int argc, char **argv) {
	 int dest;
	  int i;
	  int id;
	  int ierr;
	  int p;
	  struct
	  {
	    int x;
	    int y;
	    int z;
	  } point;
	  MPI_Datatype point_type;
	  int source;
	  MPI_Status status;
	  int tag;
	//
	//  Initialize MPI.
	//
	  ierr = MPI_Init ( &argc, &argv );
	//
	//  Get the number of processes.
	//
	  ierr = MPI_Comm_size ( MPI_COMM_WORLD, &p );
	//
	//  Get the individual process ID.
	//
	  ierr = MPI_Comm_rank ( MPI_COMM_WORLD, &id );
	//
	//  Master
	//
	  if ( id == 0 )
	  {

	    printf("An MPI example program that uses an MPI datatype.\n The number of processes is %d \n This is %d\n",p,id);

	    GtkWidget *window;
	    gtk_init(&argc, &argv);

	    //the particular problem/////////////
	    PAproblem pap;
	    create_aproblem_window(&window);
	    /////////////////////////////////////

	    gtk_main();

	  }
	//
	//  Define and commit the new datatype.
	//
	  ierr = MPI_Type_contiguous ( 3, MPI_INT, &point_type );
	  ierr = MPI_Type_commit ( &point_type );

	  if ( id == 0 )
	  {
	    point.x = 1;
	    point.y = 2;
	    point.z = 4;
	    dest = 1;
	    tag = 1;

	    ierr = MPI_Send ( &point, 1, point_type, dest, tag, MPI_COMM_WORLD );

	    printf("Process %d sent a POINT_TYPE: %d, %d, %d\n",id,point.x,point.y,point.z);

	    source = 1;
	    tag = 2;
	    ierr = MPI_Recv ( &point, 1, point_type, source, tag, MPI_COMM_WORLD,
	      &status );

	    printf("Process %d received a modified POINT_TYPE: %d, %d, %d\n",id,point.x,point.y,point.z);

	  }
	  else if ( id == 1 )
	  {
	    source = 0;
	    tag = 1;

	    ierr = MPI_Recv ( &point, 1, point_type, source, tag, MPI_COMM_WORLD,
	      &status );

	    i = point.x;
	    point.x = point.z * 100;
	    point.y = point.y * 10;
	    point.z = i;
	    dest = 0;
	    tag = 2;

	    ierr = MPI_Send ( &point, 1, point_type, dest, tag, MPI_COMM_WORLD );

	    if ( ierr != 0 )
	    {
	    	printf("\n MPI_Send returns error %d",ierr);

	      return 1;
	    }

	  }
	  else
	  {
	    printf("I am %d and MPI has nothing for me to do!\n",id);
	  }
	//
	//  Terminate MPI.
	//
	  MPI_Finalize ( );
	//
	//  Terminate.
	//
	  if ( id == 0 )
	  {

	    printf("  Normal end of execution.\n");


	  }

    return 0;
}

//	/*tests*//////////////
////	testInit1();
////	testInit2();
////	testInit3();
////	testInit4();
////	//aproblem
////	testInit5();
////	testInit6();
//////	testInit6b();
////
////	testInit7();
////	testInit8();
//	testInit9();

