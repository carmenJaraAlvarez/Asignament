#include <stdio.h>
#include <string.h>
#include "../src/common/cadena.h"
#include "../src/assignment/aproblem.h"
#include "../src/assignment/task.h"
#include "../src/assignment/resource.h"
#include "../src/assignment/PD/a_problem_PD.h"
#include "../src/assignment/solution.h"
void testInitAProblemPD(PAproblemPD, Aproblem);
void testInit7();
void testInit8();


void testInitAProblemPD(PAproblemPD papd, Aproblem a){
	printf("\n*** testInitAProblemPD ***\n");
	int res=-1;
	res = initAProblemPD(papd, &a);

	if(res == -1) {
			perror("error initAProblemPD.\n");
		} else {
			printf("ok initAProblemPD.\n");
			printf("index: %d\n",papd->index);
		}
}


void testInit7(){
	printf("\n7***********************\n");
	Cadena url="/home/practica/eclipse-workspace/c/files/data3.txt";
	Aproblem ap;
	int numTasks=3;
	int numResources=3;
	read_aproblem_file(&ap, numTasks, numResources, url);
	AproblemPD appd;
	testInitAProblemPD(&appd, ap);
	show_aproblem(&(appd.aproblem));
//	deleteAProblem(&ap);
	delete_problem_PD(&appd);
}
void testInit8(){
	printf("\n8***********************\n");
	Cadena url="/home/practica/eclipse-workspace/c/files/data3.txt";
	Aproblem ap;
	int numTasks=3;
	int numResources=3;
	read_aproblem_file(&ap, numTasks, numResources, url);

	//double vector[9]={1.,20000.,3.,0.,0.,1.,9.,8.,700.};
	//testInitAProblem(&ap,tasks,resources, numTasks,numResources,vector);
	AproblemPD appd;
	testInitAProblemPD(&appd, ap);
	show_aproblem(&(appd.aproblem));
	Logico b=is_base_case(&appd);
	printf("is base case...%d\n", b);
	//change to base case
	Solution sol;
	init_solution(&sol,numTasks);
	Alternative a;
	for(int i=0;i<2;i++){
		a.indexResource=i;
		update_solution(&sol, &a, i,appd.aproblem);
	}
	appd.solution=sol;
	appd.index=2;

	b=is_base_case(&appd);
	printf("is base case...%d\n", b);
	SpPD sp;
	if(b){
		printf("getting base case solution...\n");
		get_solution_base_case(&appd, &sp);
		//solutionToString(appd.solution);
		printf("Partial solution: resource %d \nValue %f\n",sp.alternative.indexResource, sp.value);
		combine_solutions(appd,&sol,&sp);
		printf("combined solution value %f\n", sol.acum);
		AproblemPD new;
		int i=get_num_subproblems();
		get_subproblem(&appd, &new,a, i);
		show_aproblem(&(new.aproblem));
//		deleteAProblem(&ap);
		delete_problem_PD(&appd);

	}



}

