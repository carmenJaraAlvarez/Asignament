/*
 * task.c
 *
 *  Created on: 22 dic. 2020
 *      Author: practica
 */

#include "task.h"

 static Logico check_task_name(const Cadena);


  int init_task(PTask pt,const Cadena name){
	  int res=-1;
	  if(!check_task_name(name))
	  {
		  printf("error: no task's name\n");
	  }
	  else
	  {
		  res=0;
		  strcpy (pt->name, name);
	  }
	  return res;
  }

  void show_task(const PTask t)
  {
	  printf("Task: %s", t->name);

  }

  void show_tasks(const PTask res, const int numr)
  {
	  	int i;
	  	for(i=0;i<numr;i++)
	  	{
	  		printf("Task %d:",i+1);
	  		show_task(&res[i]);
	  	}
  }
/***************************************/

  static Logico check_task_name(const Cadena name)
  {
	  Logico res=FALSE;
	  if(name!=NULL && strlen(name)>0)
	  {
		  res=TRUE;
	  }
	  return res;
  }

