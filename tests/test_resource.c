#include <stdio.h>
#include <string.h>
#include "../src/common/cadena.h"
#include "../src/assignment/resource.h"


void testInitResource(PResource pr,const Cadena name);
void testInit1();
void testInit2();


void testInitResource(PResource pr,const Cadena name){
	printf("*** testInitResource ***\n");
	int res = initResource(pr, name);

		if (res == -1) {
			printf("error initResource.\n");
		} else {
			printf("ok initResource.\n");
			showResource(pr);
			printf("\n");
		}


}


void testInit1(){
	Resource r;
	testInitResource(&r,"");
}
void testInit2(){
	Resource r;
	testInitResource(&r,"R1");
}
