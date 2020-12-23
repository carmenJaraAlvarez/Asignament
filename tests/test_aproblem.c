#include <stdio.h>
#include <string.h>
#include "../src/common/cadena.h"
#include "../src/assignment/aproblem.h"
#include "../src/assignment/task.h"
#include "../src/assignment/resource.h"


void testInitAProblem(PAproblem pa,ArrayPTasks tasks,ArrayPResources resources,int numTasks, int numResources, double values[]);
void testInit5();
void testInit6();


void testInitAProblem(PAproblem pa,ArrayPTasks tasks,ArrayPResources resources,int numTasks, int numResources, double *values){
	printf("*** testInitAProblem ***\n");
	int res=-1;
	res = initAProblem(pa, tasks, resources, numTasks, numResources,values);
	if(res == -1) {
			printf("error initAProblem.\n");
		} else {
			printf("ok initAProblem.\n");
			printf("\n");
		}
}


void testInit5(){
	Aproblem ap;
	ArrayPTasks tasks={};
	ArrayPResources resources={};
	int numTasks=3;
	int numResources=3;
	double vector[9]={1.,200.,3.,0.,0.,1.,9.,8.,7.};
	testInitAProblem(&ap,tasks,resources, numTasks,numResources,vector);
	showAproblem(&ap);
}
void testInit6(){
	Aproblem ap;
	int numTasks=3;
	int numResources=3;
	Cadena url="/home/practica/eclipse-workspace/c/files/data1.txt";
	readAproblemFile(&ap, numTasks, numResources, url);
	printf("Reading problem...");
	showAproblem(&ap);
}
