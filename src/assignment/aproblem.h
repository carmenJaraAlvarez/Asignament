#ifndef INCLUDES_APROBLEM_H_
#define INCLUDES_APROBLEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "mpe.h"
#include "mpe_log.h"
#include "../common/logico.h"
#include "../common/cadena.h"
#include "../common/type.h"
#include "solution.h"
#include "task.h"

extern int print_all;


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
	Type type;//type=MAX;//default


  } Aproblem;

  typedef Aproblem* PAproblem;

  int init_aproblem(PAproblem,const PTask,const PResource,const int, const int, const double values[]);
  int delete_aproblem(PAproblem);//free memory init
  void show_aproblem(const PAproblem);
  int read_aproblem_file(PAproblem pap, const int numTasks, const int numResources,const Cadena url);
  Type get_aproblem_type(const PAproblem);
  int get_max_num_problems(const PAproblem);
  int get_max_num_alternatives(const PAproblem);


  
  #endif /* INCLUDES_APROBLEM_H_ */
