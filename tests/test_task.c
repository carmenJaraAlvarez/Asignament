#include <stdio.h>
#include <string.h>
#include "../src/common/cadena.h"
#include "../src/assignment/task.h"


void testInitTask(PTask pt,const Cadena name);
void testInit3();
void testInit4();


void testInitTask(PTask pt,const Cadena name){

	int res = init_task(pt, name);

		if (res == -1) {
			perror("error initTask.\n");
		} else {
			printf("ok initTask\n");
			show_task(pt);
			printf("\n");
		}


}


void testInit3(){
	printf("\n3***********************\n");
	Task t;
	printf("\nexpected: error\n");
	testInitTask(&t,"");
}
void testInit4(){
	printf("\n4***********************\n");
	Task t;
	testInitTask(&t,"T1");
}
