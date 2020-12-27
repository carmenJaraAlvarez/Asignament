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
#include "../../PD/Sp_PD.h"

typedef struct
  {
	Aproblem aproblem;
	int index;
	Solution solution;

  } AproblemPD;

typedef AproblemPD* PAproblemPD;
typedef PAproblemPD ArrayPAproblemPD[TAM_ARRAY_PROBLEM];

  int initAProblemPD(PAproblemPD,Aproblem);
  Type get_type(PAproblemPD);
  Logico is_base_case(PAproblemPD);
  int get_alternatives(PAproblemPD, ArrayAlternatives);
  Logico is_alternative(PAproblemPD, int);
  int select_alternative(PAproblemPD,ArrayAlternatives, int,double*);
  int get_solution_base_case(PAproblemPD,PSpPD);
  int combine_solutions(AproblemPD, PSolution,SpPD);
  int get_num_subproblems();//here is 1
  int get_subproblem(const PAproblemPD, PAproblemPD,Alternative, int);


  /*
 get_estimated_result();
  get_solution();
  */




#endif /* ASSIGNMENT_PD_A_PROBLEM_PD_H_ */
