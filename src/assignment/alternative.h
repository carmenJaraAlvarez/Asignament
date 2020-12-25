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

	int indexResource;//index resource, in this problem

  } Alternative;

  typedef Alternative* PAlternative;
  typedef PAlternative ArrayPAlternatives[TAM_ARRAY_ALTERNATIVE];

  int initAlternative(PAlternative,int);

  #endif /* ASSIGNMENT_ALTERNATIVE_H_ */
