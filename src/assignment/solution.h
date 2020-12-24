/*
 * Solution.h
 *
 *  Created on: 24 dic. 2020
 *      Author: practica
 */

#ifndef ASSIGNMENT_SOLUTION_H_
#define ASSIGNMENT_SOLUTION_H_
#include "resource.h"
#include "task.h"

typedef struct
  {
	//same position in both arrays is an assigned pair
	ArrayPTasks tasks;//is not necessary if we take initial task enum
	ArrayPResources resources;

	int lengthArrays;

	double acum;//Sum values assigned pairs

  } Solution;

  typedef Solution* PSolution;
  typedef PSolution ArrayPSolutions[TAM_ARRAY_PROBLEM];

  int solutionToString(PSolution);

#endif /* ASSIGNMENT_SOLUTION_H_ */
