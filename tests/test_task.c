#include <stdio.h>
#include <string.h>
#include "../src/common/cadena.h"
#include "../src/assignment/task.h"


void testInitTask(PTask pt,const Cadena name);
void testInit3();
void testInit4();


void testInitTask(PTask pt,const Cadena name){
	printf("*** testInitTask ***\n");
	int res = initTask(pt, name);

		if (res == -1) {
			printf("error initTask.\n");
		} else {
			printf("ok initTask\n");
			showTask(pt);
			printf("\n");
		}


}


void testInit3(){
	Task t;
	testInitTask(&t,"");
}
void testInit4(){
	Task t;
	testInitTask(&t,"T1");
}
