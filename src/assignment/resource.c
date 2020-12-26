/*
 * resource.c
 *
 *  Created on: 22 dic. 2020
 *      Author: practica
 */
#include "resource.h"

  int init_resource(PResource pr,const Cadena name){
	  int res=-1;
	  if(!check_name(name)){
		  printf("error: no resource's name\n");
		  	  }
	  else{
		  res=0;
		  strcpy (pr->name, name);
	  }
	  return res;
  }
  void showResource(PResource r){
	  printf("Resource: %s", r->name);

  }
  void showResources(ArrayPResources res, int numr){
	  	int i;
	  	for(i=0;i<numr;i++){
	  		printf("Resource %d:",i+1);
	  		showResource(res[i]);
	  	}
	  }



  Logico check_name(Cadena name){
	  Logico res=FALSE;
	  if(name!=NULL && strlen(name)>0){
		  res=TRUE;
	  }
	  return res;
  }
