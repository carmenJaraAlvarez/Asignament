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
		int alternative;
		double values[50];//TODO

	  }w_mpi;


int distribution(PalgorithmPD, int);
int rcv_work();
int send_work(PalgorithmPD,int, int);
void waitting_answer();

#endif /* MPI_PROBLEM_MPI_H_ */
