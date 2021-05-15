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
extern GtkWidget *window;
extern Cadena test;

int test_set(int np);



int test_set(int np){
	printf("\n*** test_set ***\n");
	test_set_data(test,np);
	//test_set_data(4,np);
	return 0;
}
int test_set_data(Cadena t, int np){
	printf("\n*** test_set_data***\n");
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
	if(1)
	{
		printf("\ntest_application.c		test_set_data()-> READED file  %d\n",e);
		g_print ("\ntest_application.c		test_set_data()		prune->%d",prune);
		g_print ("\ntest_application.c		test_set_data()	rr->%d",redistribution_rr);
	}

		resolve_aPD(&pap_from_gui, np);
		printf("\n*******************************************"
				"\ntest_application.c		test_set_data(%d)-> acum %f\n",i,final_alg.best);
		//gtk_window_close (&window);
		return 0;

}

