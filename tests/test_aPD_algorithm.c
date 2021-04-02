#include <stdio.h>
#include <string.h>
#include "../src/PD/PD_algorithm.h"
#include "../src/common/cadena.h"
#include "../src/assignment/aproblem.h"
#include "../src/assignment/task.h"
#include "../src/assignment/resource.h"
#include "../src/assignment/PD/a_problem_PD.h"
#include "../src/assignment/solution.h"

void testInitAProblemPD(PAproblemPD, Aproblem);
void testInit9();
void testInit10();


//void testInitAProblemPD(PAproblemPD papd, Aproblem a){
//	printf("\n*** testInitAProblemPD ***\n");
//	int res=-1;
//	res = initAProblemPD(papd, &a);
//
//	if(res == -1) {
//			perror("error initAProblemPD.\n");
//		} else {
//			printf("ok initAProblemPD.\n");
//			printf("index: %d\n",papd->index);
//		}
//}

void testInit9(){
	printf("\n 9 **********************\n");
	//mpecc -mpilog -pthread
	Cadena url="/home/practica/eclipse-workspace/c/files/data3.txt";

	Aproblem ap;
	int numTasks=3;
	int numResources=3;
	printf("%d , %d\n ",numTasks,numResources);
	read_aproblem_file(&ap, numTasks, numResources, url);
	AproblemPD appd;
	testInitAProblemPD(&appd, ap);
	AlgorithmPD alg;
	printf("\n 9 \n");
	init_algorithmPD(&alg, appd);

	printf("best post init: %f",alg.best);
	show_aproblem(&(alg.ppd.aproblem));
	exec_algorithm(&alg);
	printf("\nAfter exec algoritm\n");
	Solution sol;
	get_PDsolution(&alg, &sol);
	for(int i=0;i<alg.ppd.solution.lengthArrays;i++){
		printf("\nResources: \n*%s\n",alg.ppd.solution.resources[i].name);
	}
	printf("Solution value: %f", alg.ppd.solution.acum);
	delete_algorithmPD(&alg);
}
void testInit10(){
	printf("\n10***********************\n");

}
