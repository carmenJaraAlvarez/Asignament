/*
 * PD_algorithm.h
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */



#ifndef PD_PD_ALGORITHM_H_
#define PD_PD_ALGORITHM_H_


typedef struct
  {

   //ProblemPD ppd;
   double best;


  } AlgorithmPD;
  typedef AlgorithmPD* PalgorithmPD;


  int updateBest(PalgoritmPD);
  int getSolution(PalgoritmPD, Psolution);
  int pD(PalgorithPD);

#endif /* PD_PD_ALGORITHM_H_ */
