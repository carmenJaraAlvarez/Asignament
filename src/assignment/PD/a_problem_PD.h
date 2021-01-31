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

#define GREAT 999999.
#define SMALL -999999.
typedef struct
  {
	Aproblem aproblem;
	int index;
	Solution solution;

  } AproblemPD;

typedef AproblemPD* PAproblemPD;


  int initAProblemPD(PAproblemPD,PAproblem);
  Type get_type(const PAproblemPD);
  Logico is_base_case(const PAproblemPD);
  int get_alternatives(const PAproblemPD, PAlternative);//PAlternative is a pointer to a dinamic array
  Logico is_alternative(const PAproblemPD, int);
  int select_alternative(PAproblemPD,PAlternative, int,double*);//PAlternative is a pointer to a dinamic array
  int get_solution_base_case(PAproblemPD,PSpPD);
  int combine_solutions(AproblemPD, PSolution,PSpPD);
  int get_num_subproblems();//here is 1
  int get_subproblem(const PAproblemPD, PAproblemPD,Alternative, int);
  double get_estimate(PAproblemPD);
  int get_solution(AproblemPD);
  double get_target(PAproblemPD);
  int get_size(const PAproblemPD);
  int update_solution(PAproblemPD, PAlternative, double ,Aproblem);
  int delete_problem_PD(PAproblemPD);//free memory
  int copy_aproblem_PD(PAproblemPD,AproblemPD);
  //public aux
  int show_aproblem_PD(PAproblemPD);





#endif /* ASSIGNMENT_PD_A_PROBLEM_PD_H_ */
