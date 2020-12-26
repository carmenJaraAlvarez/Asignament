/*
 * alternative.h
 *
 *  Created on: 24 dic. 2020
 *      Author: practica
 */

#ifndef ASSIGNMENT_ALTERNATIVE_H_
#define ASSIGNMENT_ALTERNATIVE_H_


#define TAM_ARRAY_ALTERNATIVE 100

typedef struct
  {

	int indexResource;//position in matrix, in this problem

  } Alternative;

  typedef Alternative* PAlternative;
  typedef Alternative ArrayAlternatives[TAM_ARRAY_ALTERNATIVE];

  int initAlternative(PAlternative,int);

  #endif /* ASSIGNMENT_ALTERNATIVE_H_ */
