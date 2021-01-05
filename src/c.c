/*
 ============================================================================
 Name        : c.c
 Author      : cj

 ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

#include "common/logico.h"
#include "assignment/aproblem.h"
#include "../tests/test_resource.c"
#include "../tests/test_task.c"
#include "../tests/test_aproblem.c"
#include "../tests/test_aproblemPD.c"
#include "../tests/test_aPD_algorithm.c"

int main(int argc, char* argv[]){
	/*tests*//////////////
//	testInit1();
//	testInit2();
//	testInit3();
//	testInit4();
//	//aproblem
//	testInit5();
//	testInit6();
////	testInit6b();
//
//	testInit7();
//	testInit8();
	testInit9();
	//num nodes
/*	int totalProcess=2;
	//gui
	//get data
	int N=3;
	int M=3;
	double vector[]={1.,2.,3.,0.,0.,1.,9.,8.,7.};
	//dinamic size
	//vector de vectores. Cada fila se gestiona con puntero y le matriz es un array de esos punteros
	//dinamic_matrix[0]=puntero al vector de enteros de la primera fila

	double **dinamic_matrix;
	dinamic_matrix=malloc(M*sizeof(double *));
	for (int i=0; i<M;i++){
		dinamic_matrix[i]=malloc(N*sizeof(double *));
	}
	/////
	int k=0;
	for (int i=0;i<M;i++){
		for(int j=0;j<N;j++){
			double aux=vector[k];
			dinamic_matrix[i][j]=aux;
			k++;
		}

	}

	//crea problema inicio

	Gproblem gp;
	initGProblem(&gp, M, N, dinamic_matrix);
	showGproblem(gp);*/

	return 0;
}
