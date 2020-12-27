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
	ArrayAproblemPD problems;
//***************************************************
	int num_problemas;
  } AlgorithmPD;
  typedef AlgorithmPD* PalgorithmPD;

  Logico is_min(PalgorithmPD);
  Logico is_max(PalgorithmPD);
  int init_algorithmPD(PalgorithmPD, ArrayAproblemPD);
  int randomize(PalgorithmPD,ArrayAlternatives);
  int exec_algorithm(PalgorithmPD);
  int updateBest(PalgorithmPD);
  int get_PDsolution(PalgorithmPD, PSolution);
  int pD(PalgorithmPD, PSpPD);

#endif /* PD_PD_ALGORITHM_H_ */
