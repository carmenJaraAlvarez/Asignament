#ifndef INCLUDES_PROBLEM_PD_H_
#define INCLUDES_PROBLEM_PD_H_

#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "../common/logico.h"

#define TAM_ARRAY 100


typedef struct
  {

    enum{MAX,MIN,OTHER} type;

  } ProblemPD;
  typedef ProblemPD* PproblemPD;
  typedef PproblemPD ArrayPproblemPDs[TAM_ARRAY];

  int getTypePD(ProblemPD);


  
  #endif /* INCLUDES_PROBLEM_PD_H_ */
