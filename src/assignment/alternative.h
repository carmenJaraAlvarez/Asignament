/*
 * alternative.h
 *
 *  Created on: 24 dic. 2020
 *      Author: practica
 */

#ifndef ASSIGNMENT_ALTERNATIVE_H_
#define ASSIGNMENT_ALTERNATIVE_H_

#include "resource.h"
#define TAM_ARRAY_ALTERNATIVE 100

typedef struct
  {

	ArrayPResources resources;

	int lengt;

  } Alternative;

  typedef Alternative* PAlternative;
  typedef PAlternative ArrayPAlternatives[TAM_ARRAY_ALTERNATIVE];



#endif /* ASSIGNMENT_ALTERNATIVE_H_ */
