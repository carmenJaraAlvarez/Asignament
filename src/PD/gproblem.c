/*		gproblem
 *      Author: carmen
 */
#include "gproblem.h"
#include "../common/logico.h"
int initGProblem(PGproblem res, int N, int M, double *dinamic_matrix[]){
//checking ok>>ini
	int result=0;
	if(!checkN(N)){
		printf("error N: %d", N);
		result=-1;
	}
	if(!checkM(M)){
		printf("error M");
		result=-1;
	}
//	if(!checkMatrix(matrix)){
//		printf("error matrix");
//		result=-1;
//	}
	else{
		res->N=N;
		res->M=M;
		int i=0;
		int j=0;
		for(i=0;i<M;i++){
			for(j=0;j<N;j++){
				res->matrix[i][j]=dinamic_matrix[i][j];
			}
		}

	}
	return result;
}
void showGproblem(Gproblem a){

//	N: 3, M: 3, Values: 1 2 3 4 5 6 7 8 9
	int M=a.M;
	int N=a.N;

	for(int i=0;i<M;i++){

		for(int j=0; j<N; j++){
			double aux=a.matrix[i][j];
			printf("%f  ", aux);
		}
		printf("\n");
	}



}
//void showGProblems(const ArrayPGproblems res, int nGP){
//	int i;
//	for(i=0;i<nGP;i++){
//		printf("Problem %d:",i+1);
////		showGproblem(res[i]);
//	}
//}

//int readMatrixFile(){
////	Lee desde un fichero de texto y las almacena en un array. Devuelve el n�mero de
////	elementos leidos, o 0 si no se encuentra el fichero.
//	//quitarSaltoDeLinea en cadena.h//
//	int result=0;
//	int i;
//	Gproblem gp;
//	Cadena filename;
//	FILE* f;
//	f=fopen(filename, "r");
//	while(!feof(f)){
//		for(i=0;i<5;i++){
//
//
//
//
//			i++;
//		}
//
//	}
//	fclose(f);
//
//
//	return result;
//}
//auxiliares
Logico checkN(int N){//
	Logico res=FALSE;
	if(N>0){//strlen no considera \0
		res=TRUE;
	}
	return res;
}
Logico checkM(int M){
	Logico res=FALSE;
		if( M>0 ){
			res=TRUE;
		}
	return res;
}
//Logico checkMatrix(int matrix[3][3]){
//	Logico res=FALSE;
//	if(matrix[3][3]){
//		res=TRUE;
//	}
//	return res;
//
//}
