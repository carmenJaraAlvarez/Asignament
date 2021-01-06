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




#define TAM_MAX_READ 100//length names


typedef struct
  {
//tasks array
	Task *tasks;
//resources array
	Resource *resources;
	int numTask;
	int numResources;
//values matrix task*resources
	double *values;
//problem type: min, max, other
	Type type;


  } Aproblem;

  typedef Aproblem* PAproblem;


  int initAProblem(PAproblem,PTask,PResource,int, int, double values[]);
  int deleteAProblem(PAproblem);//free memory init
  void showAproblem(PAproblem);
  void showAproblems();
  int readAproblemFile(PAproblem pap, const int numTasks, const int numResources,const Cadena url);
  Type getAproblemType(PAproblem);
  int get_max_num_problems(PAproblem);
  int get_max_num_alternatives(PAproblem);


  Logico checkTasks(ArrayTasks);
  Logico checkResources(ArrayResources);
  Logico checkMatrix(double values[]);
  Logico checkInt(int);


  
  #endif /* INCLUDES_APROBLEM_H_ */
