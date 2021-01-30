/*
 * resource.c
 *
 *  Created on: 22 dic. 2020
 *      Author: practica
 */
#include "resource.h"

static Logico check_name(const Cadena);

  int init_resource(PResource pr,const Cadena name,int position)
  {
	  int res=-1;
	  if(!check_name(name))
	  {
		  printf("error: no resource's name\n");
	  }
	  else
	  {
		  res=0;
		  strcpy (pr->name, name);
		  pr->position=position;
	  }
	  return res;
  }

  void show_resource(PResource r)
  {
	  printf("Resource %d: %s",r->position, r->name);

  }
/****************************************/
  static Logico check_name(const Cadena name)
  {
	  Logico res=FALSE;
	  if(name!=NULL && strlen(name)>0)
	  {
		  res=TRUE;
	  }
	  return res;
  }
