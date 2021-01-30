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



typedef struct
  {
	int position;//in initial problem resources
   	Cadena name;

  } Resource;

  typedef Resource* PResource;


  int init_resource(PResource,const Cadena, int position);
  void show_resource(PResource);




#endif /* ASSIGNMENT_RESOURCE_H_ */
