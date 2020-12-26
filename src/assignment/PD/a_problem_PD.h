/*
 * a_problem_PD.h
 *
 *  Created on: 22 dic. 2020
 *      Author: practica
 */

#ifndef ASSIGNMENT_PD_A_PROBLEM_PD_H_
#define ASSIGNMENT_PD_A_PROBLEM_PD_H_
#include "../../common/logico.h"
#include "../aproblem.h"
#include "../alternative.h"
#include "../solution.h"

typedef struct
  {
	Aproblem aproblem;
	int index;
	Solution solution;

  } AproblemPD;

typedef AproblemPD* PAproblemPD;
typedef PAproblemPD ArrayPAproblemPD[TAM_ARRAY_PROBLEM];

  int initAProblemPD(PAproblemPD,Aproblem);
  Logico is_base_case(PAproblemPD);
  int get_alternatives(PAproblemPD, ArrayAlternatives);
  int get_solution_base_case(PAproblemPD);
  int get_subproblem(const PAproblemPD fatherProblem, PAproblemPD newProblem,Alternative a );
  int select_alternative(PAproblemPD,ArrayAlternatives, int,double*);
  //check resource with index i, is an option
  Logico is_alternative(PAproblemPD, int);
  /*
  get_tipo();
  new_solution();
  get_estimated_result();
  get_solution();
  */




#endif /* ASSIGNMENT_PD_A_PROBLEM_PD_H_ */
