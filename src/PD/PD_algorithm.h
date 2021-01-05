/*
 * PD_algorithm.h
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */



#ifndef PD_PD_ALGORITHM_H_
#define PD_PD_ALGORITHM_H_

//include path to particular PD problem
//***************************************************
#include "../assignment/PD/a_problem_PD.h"
//***************************************************

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

  Logico is_min(PalgorithmPD);
  Logico is_max(PalgorithmPD);
  int init_algorithmPD(PalgorithmPD, AproblemPD);
  int randomize(PalgorithmPD,PAlternative);
  int exec_algorithm(PalgorithmPD);
  int update_best(PalgorithmPD);
  int get_PDsolution(PalgorithmPD, PSolution);
  int pD(PalgorithmPD);
  int getPreviousProblems(PalgorithmPD,ArrayAproblemPD);
  int delete_algorithmPD(PalgorithmPD);

#endif /* PD_PD_ALGORITHM_H_ */
