/*
 * problem_MPI.h
 *
 *  Created on: 13 ene. 2021
 *      Author: practica
 */

#ifndef MPI_PROBLEM_MPI_H_
#define MPI_PROBLEM_MPI_H_
#include "mpi.h"
#include "../PD/PD_algorithm.h"



  struct  Work
	  {
		int num_tasks;
		int num_resources;
		//problem type: min, max, other
		Type type;
		//alternative the process will select
		int num_alternatives;



	  }w_mpi;



int distribution(PalgorithmPD, int);
int rcv_work();
int init_work(PAproblem, int, int*);
int send_work(PalgorithmPD,int *, int, int);
void waitting_answer();
int serializer_tasks(PalgorithmPD, char*);
int deserializer_tasks(char*, int, PTask );
int serializer_resources(PalgorithmPD palg, char* all);
int deserializer_resources(char*, int, PResource);

#endif /* MPI_PROBLEM_MPI_H_ */
