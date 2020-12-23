#ifndef INCLUDES_APROBLEM_H_
#define INCLUDES_APROBLEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "../common/logico.h"
#include "../common/cadena.h"
#include "task.h"
#include "resource.h"

#define TAM_ARRAY_PROBLEM 130
#define TAM_MAX_READ 100


typedef struct
  {
//tasks array
	ArrayPTasks tasks;
	ArrayPResources resources;
	int numTask;
//resources array

	int numResources;
//values matrix task*resources
	double values[TAM_ARRAY_PROBLEM];

  } Aproblem;

  typedef Aproblem* PAproblem;
  typedef PAproblem ArrayPAproblems[TAM_ARRAY_PROBLEM];

  int initAProblem(PAproblem,ArrayPTasks,ArrayPResources,int, int, double values[]);
  void showAproblem(PAproblem);
  void showAproblems();
  int readAproblemFile(PAproblem pap, const int numTasks, const int numResources,const Cadena url);


  Logico checkTasks(ArrayPTasks);
  Logico checkResources(ArrayPResources);
  Logico checkMatrix(double values[]);
  Logico checkInt(int);


  
  #endif /* INCLUDES_APROBLEM_H_ */
