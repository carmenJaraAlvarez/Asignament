/*
 * alternative.h
 *
 *  Created on: 24 dic. 2020
 *      Author: practica
 */

#ifndef ASSIGNMENT_ALTERNATIVE_H_
#define ASSIGNMENT_ALTERNATIVE_H_


typedef struct
  {

	int indexResource;//position in matrix, in this problem

  } Alternative;

  typedef Alternative* PAlternative;


  int initAlternative(PAlternative,int);
  int init_alternative_array(Alternative**,int);//int parameter is the max number of alternatives
  int delete_alternatives(PAlternative*);//free memory

  #endif /* ASSIGNMENT_ALTERNATIVE_H_ */
