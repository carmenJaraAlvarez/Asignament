#include <stdio.h>
#include <string.h>
#include "../src/common/cadena.h"
#include "../src/assignment/resource.h"


void testInitResource(PResource pr,const Cadena name);
void testInit1();
void testInit2();


void testInitResource(PResource pr,const Cadena name){

	int res = init_resource(pr, name);

		if (res == -1) {
			perror("error initResource.\n");
		} else {
			printf("ok initResource.\n");
			show_resource(pr);
			printf("\n");
		}


}


void testInit1(){
	printf("\n1***********************\n");
	Resource r;
	printf("\nexpected: error\n");
	testInitResource(&r,"");
}
void testInit2(){
	printf("\n2***********************\n");
	Resource r;
	testInitResource(&r,"R1");
}
