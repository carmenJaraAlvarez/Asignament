#ifndef INCLUDES_APROBLEM_H_
#define INCLUDES_APROBLEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "../common/logico.h"
#include "../common/cadena.h"
#include "../common/type.h"
#include "solution.h"
#include "task.h"



#define TAM_ARRAY_PROBLEM 130
#define TAM_MAX_READ 100


typedef struct
  {
//tasks array
	ArrayTasks tasks;
//resources array
	ArrayResources resources;
	int numTask;
	int numResources;
//values matrix task*resources
	double values[TAM_ARRAY_PROBLEM];
//problem type: min, max, other
	Type type;


  } Aproblem;

  typedef Aproblem* PAproblem;
  typedef PAproblem ArrayPAproblems[TAM_ARRAY_PROBLEM];

  int initAProblem(PAproblem,ArrayTasks,ArrayResources,int, int, double values[]);
  void showAproblem(Aproblem);
  void showAproblems();
  int readAproblemFile(PAproblem pap, const int numTasks, const int numResources,const Cadena url);
  Type getAproblemType(Aproblem);

  Logico checkTasks(ArrayTasks);
  Logico checkResources(ArrayResources);
  Logico checkMatrix(double values[]);
  Logico checkInt(int);


  
  #endif /* INCLUDES_APROBLEM_H_ */
