/*
 * task.c
 *
 *  Created on: 22 dic. 2020
 *      Author: practica
 */

#include "task.h"

  int init_task(PTask pt,const Cadena name){
	  int res=-1;
	  if(!check_name(name)){
		  printf("error: no task's name\n");
		  	  }
	  else{
		  res=0;
		  strcpy (pt->name, name);
	  }
	  return res;
  }
  void showTask(PTask t){
	  printf("Task: %s", t->name);

  }
  void showTasks(ArrayPTasks res, int numr){
	  	int i;
	  	for(i=0;i<numr;i++){
	  		printf("Task %d:",i+1);
	  		showTask(res[i]);
	  	}
	  }



  Logico check_task_name(Cadena name){
	  Logico res=FALSE;
	  if(name!=NULL && strlen(name)>0){
		  res=TRUE;
	  }
	  return res;
  }

