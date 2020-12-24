/*
 * PD_algorithm.h
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */



#ifndef PD_PD_ALGORITHM_H_
#define PD_PD_ALGORITHM_H_

#include "problem_PD.h";

typedef struct
  {

   ProblemPD ppd;
   double best;


  } AlgorithmPD;
  typedef AlgorithmPD* PalgorithmPD;
  typedef PalgorithmPD ArrayPalgorithmPDs[TAM_ARRAY];

  int updateBest(PalgoritmPD);
  int getSolution(PalgoritmPD, Psolution);
  int pD(PalgorithPD);

#endif /* PD_PD_ALGORITHM_H_ */
