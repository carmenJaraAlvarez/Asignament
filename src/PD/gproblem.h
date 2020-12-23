#ifndef INCLUDES_GPROBLEM_H_
#define INCLUDES_GPROBLEM_H_

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "../common/logico.h"

#define TAM_ARRAY 100
#define TAM_CODIGO 8

typedef struct
  {

    int N;
    int M;
    double matrix[3][3];//TODO

  } Gproblem;
  typedef Gproblem* PGproblem;
  typedef PGproblem ArrayPGproblems[TAM_ARRAY];

  int initGProblem(PGproblem, int,  int, double *dinamic_matrix[]);
  void showGproblem(Gproblem);
  //void showGproblems(const ArrayPGproblems, int);
  //int readMatrixFile(int[3][3]);

  Logico checkN(int);
  Logico checkM(int);
 // Logico checkMatrix(int[3][3]);

  
  #endif /* INCLUDES_GPROBLEM_H_ */
