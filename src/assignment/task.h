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

#define TAM_ARRAY_TASKS 100
typedef struct
  {

   	Cadena name;

  } Task;

  typedef Task* PTask;
  typedef PTask ArrayPTasks[TAM_ARRAY_TASKS];
  typedef Task ArrayTasks[TAM_ARRAY_TASKS];

  int init_task(PTask,const Cadena);
  void showTask(PTask);
  void showTasks();


  Logico check_task_name(Cadena);



#endif /* ASSIGNMENT_TASK_H_ */
