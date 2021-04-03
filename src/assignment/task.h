/*
 * task.h
 *
 *  Created on: 22 dic. 2020
 *      Author: practica
 */

#include <string.h>
#include "../common/cadena.h"
#include "../common/logico.h"
#include "aproblem.h"

#ifndef ASSIGNMENT_TASK_H_
#define ASSIGNMENT_TASK_H_

extern int print_all;

typedef struct
  {

   	Cadena name;

  } Task;

  typedef Task* PTask;


  int init_task(PTask,const Cadena);
  void show_task(const PTask);
  void show_tasks(const PTask, const int);






#endif /* ASSIGNMENT_TASK_H_ */
