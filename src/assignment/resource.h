/*
 * resource.h
 *
 *  Created on: 22 dic. 2020
 *      Author: practica
 */

#ifndef ASSIGNMENT_RESOURCE_H_
#define ASSIGNMENT_RESOURCE_H_

#include <string.h>
#include "resource.h"
#include "../common/cadena.h"
#include "../common/logico.h"

#define TAM_ARRAY_RESOURCES 100

typedef struct
  {

   	Cadena name;

  } Resource;

  typedef Resource* PResource;
  typedef PResource ArrayPResources[TAM_ARRAY_RESOURCES];

  int initResource(PResource,const Cadena);
  void showResource(PResource);
  void showResources();


  Logico check_name(Cadena);


#endif /* ASSIGNMENT_RESOURCE_H_ */
