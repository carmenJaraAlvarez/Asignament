/*
 * Solution.h
 *
 *  Created on: 24 dic. 2020
 *      Author: practica
 */

#ifndef ASSIGNMENT_SOLUTION_H_
#define ASSIGNMENT_SOLUTION_H_
#include "resource.h"
//#include "task.h"
#include "alternative.h"
#define TAM_ARRAY_SOLUTION

typedef struct
  {

	Resource *resources;//assigned to initial tasks in same order

	int lengthArrays;

	double acum;//Sum values assigned pairs

  } Solution;

  typedef Solution* PSolution;


  int init_solution(PSolution, int);
  int delete_solution(PSolution);//let memory free
  int show_solution(const Solution);


#endif /* ASSIGNMENT_SOLUTION_H_ */
