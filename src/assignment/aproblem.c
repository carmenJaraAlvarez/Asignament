/*		gproblem
 *      Author: carmen
 */

#include "aproblem.h"




 int initAProblem(PAproblem pa,ArrayPTasks tasks,ArrayPResources resources,int numTasks, int numResources, double *values){
	 if(!checkTasks(tasks)){
		 printf("Tasks error\n");
		 return -1;
	 }
	 if(!checkResources(resources)){
		 printf("Resources error\n");
		 return -1;
	 }
	 //TODO check matrix
	 if(!checkInt(numTasks) || !checkInt(numResources)){
		 printf("Matrix dimension error\n");
		 return -1;
	 }
	 pa->numTask=numTasks;
	 pa->numResources=numResources;
	 double aux;
	 int numValues=numTasks*numResources;

	 for (int i=0;i<numValues;i++){

	 			aux=values[i];
	 			pa->values[i]=aux;

	 }
	 return 0;
 }
 void showAproblem(PAproblem pap){
	 printf("inside showAproblem");
	 int m=pap->numTask;
	 int n=pap->numResources;
	 printf("\n");
	 for(int i=0;i<m*n;i++){
		 double r=(i+1)%m;
		 if(r!=0.){

			 printf("%f ",pap->values[i]);
		 }
		 else{
			 printf("%f \n",pap->values[i]);

		 }

	 }

 }
 void showAProblems(const ArrayPAproblems res, int nGP){
 	int i;
 	for(i=0;i<nGP;i++){
 		printf("Problem %d:",i+1);
 //		showGproblem(res[i]);
 	}
 }

 int readAproblemFile(PAproblem pap, const int numTasks, const int numResources,const Cadena url){
	 int res=-1;
	 int i;
	 int j;
	 double aux;
	 Cadena c;
	 Resource resource;
	 Task task;
	 ArrayPTasks parrayT;
	 ArrayPResources parrayR;
	 int numValues=numResources*numTasks;
	 double values[numValues];
	 Aproblem ap;
	 ap.numResources=numResources;
	 ap.numTask=numTasks;
	 FILE* f;
	 f=fopen(url, "r");


	  if (f == NULL) {
	    perror("fopen");
//	    printf("error apertura fichero");
	    return -1;
	  }


	 while(!feof(f)){
		 //first rows: tasks
		 for(j=0;j<numTasks;j++){
			 fgets(c,TAM_MAX_READ,f);
			 quitaSaltoDeLinea(c);
			 initTask(&task,c);
			 parrayT[j]=&task;
		 }
		 //resources
		 for(j=0;j<numResources;j++){
			 fgets(c,TAM_MAX_READ,f);
			 quitaSaltoDeLinea(c);
			 initResource(&resource,c);
			 parrayR[j]=&resource;
		 }
		 //values

		 for(i=0;i<numValues;i++){
			 fgets(c,TAM_MAX_READ,f);
			 quitaSaltoDeLinea(c);
			 aux=atof(c);
			 values[i]=aux;
		 }
		 res=initAProblem(pap,parrayT,parrayR,numTasks,numResources, values);

	 }
	 fclose(f);
	 return res;
 }


 Logico checkTasks(ArrayPTasks tasks){
	  Logico res=FALSE;
	  if(tasks!=NULL){
		  res=TRUE;
	  }
	  return res;
 }
 Logico checkResources(ArrayPResources resources){
	  Logico res=FALSE;
	  if(resources!=NULL){
		  res=TRUE;
	  }
	  return res;
 }
 Logico checkMatrix(double values[]){
	  Logico res=FALSE;
	  if(values!=NULL){
		  res=TRUE;
	  }
	  return res;
 }
 Logico checkInt(numTasks){
	  Logico res=FALSE;
	  if(numTasks!=NULL && numTasks>0){
		  res=TRUE;
	  }
	  return res;
 }



