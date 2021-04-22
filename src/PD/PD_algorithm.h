/*
 * PD_algorithm.h
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */



#ifndef PD_PD_ALGORITHM_H_
#define PD_PD_ALGORITHM_H_
#include "../algorithms/algorithm.h"
//include path to particular PD problem
//***************************************************
#include "../assignment/PD/a_problem_PD.h"
//***************************************************
//#define NUM_MAX_SOLVED 1000


extern int numprocs;
extern int event6;//for MPE log
extern int print_all;

AproblemPD * newArrayAppd;
Transfered_nodes transfered;//if redistribution
Tuple_prune tuple_prune_data;//if tuple prune
//TODO test
int deep;

typedef struct
  {
	Logico isRandomize;
	int sizeRef;
	double target;
	double best;

//The particular PD problem
//***************************************************
	AproblemPD ppd;
	AproblemPD *problems;
	AproblemPD *solvedProblems;
//***************************************************
	int num_problems;
	int num_solved;


  } AlgorithmPD;
  typedef AlgorithmPD* PalgorithmPD;

  Logico is_min(const PalgorithmPD);
  Logico is_max(const PalgorithmPD);
  int init_algorithmPD(PalgorithmPD, AproblemPD);
  int randomize(PalgorithmPD,PAlternative);
  int exec_algorithm(PalgorithmPD,double *,MPI_Request *, int *,MPI_Request *  );
  int update_best(PalgorithmPD,const PAproblemPD);
  int get_PDsolution(PalgorithmPD, PSolution);
  int pD(PalgorithmPD,double *,MPI_Request * ,int *,MPI_Request *);
  int getPreviousProblems(PalgorithmPD,PAproblemPD);
  int delete_algorithmPD(PalgorithmPD);
  double get_best(PalgorithmPD);

  extern AlgorithmPD final_alg;
#endif /* PD_PD_ALGORITHM_H_ */
