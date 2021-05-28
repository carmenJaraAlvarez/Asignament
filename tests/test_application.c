#include <stdio.h>
#include <string.h>
#include "../src/PD/PD_algorithm.h"
#include "../src/common/cadena.h"
#include "../src/assignment/aproblem.h"
#include "../src/assignment/task.h"
#include "../src/assignment/resource.h"
#include "../src/assignment/PD/a_problem_PD.h"
#include "../src/assignment/solution.h"
#include "../src/assignment/aproblem_gui.h"

extern int redistribution_rr;
extern int redistribution_rr_all;
extern GtkWidget *window;
extern Cadena test;
extern int var_test[5];
extern int fs;
extern int type_best;



double expected[9]={2.,38.,51.,5.,1025.,46.,7.,7.,8.};//matrix size >=2	<=10


int test_set(int np);



int test_set(int np){
	if(print_all)
	{
		printf("\n*** test_set ***\n");
	}

	test_set_data(test,np);
	return 0;
}
int test_set_data(Cadena t, int np){
	if(print_all)
	{
		printf("\n*** test_set_data***\n");
	}
	printf("\n*******************************************\n");
	init_clock();
    int numt;
    int numr;
    int i=atoi(t);

	Cadena cadena_url_init="/home/practica/eclipse-workspace/c/files/data";
	Cadena cadena_url_end=".txt";
	Cadena url_center;
	numt=i;
	numr=i;

	sprintf(url_center, "%d",i);
	strcat(cadena_url_init,url_center);
	strcat(cadena_url_init,cadena_url_end);
	printf("\n%s",cadena_url_init);
	int e=read_aproblem_file(&pap_from_gui, numt, numr, cadena_url_init);

	prune=var_test[0];
	redistribution_rr=var_test[1];
	tuple_p=var_test[2];
	fs=var_test[3];
	type_best=var_test[4];
	redistribution_rr_all=var_test[5];
	if(print_all)
	{
		printf("\ntest_application.c		test_set_data()-> READED file  %d\n",e);
		g_print ("\ntest_application.c		test_set_data()		prune->%d",prune);
		g_print ("\ntest_application.c		test_set_data()	rr->%d",redistribution_rr);
	}

	resolve_aPD(&pap_from_gui, np);
	int count=0;
	while(final_alg.best!=expected[i-2])//matrix size <=2
	{
		if(count>=9000000)//Timeout
		{
			printf("\ntest_application.c		test_set_data(%d)-> TIMEOUT",i);
			break;
		}
		count++;
	}
	if(count<9000000)
	{
		printf("\ntest_application.c		test_set_data(%d)-> OK: acum %f\n",i,final_alg.best);
	}

	return 0;

}

