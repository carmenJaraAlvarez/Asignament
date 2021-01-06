/*		gproblem
 *      Author: carmen
 */

#include "aproblem.h"




 int initAProblem(PAproblem pa,PTask tasks,PResource resources,int numTasks, int numResources, double *values){
	 if(!checkTasks(tasks)){
		 printf("Tasks error\n");
		 return -1;
	 }
	 if(!checkResources(resources)){
		 printf("Resources error\n");
		 return -1;
	 }
	 //TODO check values
	 if(!checkInt(numTasks) || !checkInt(numResources)){
		 printf("Matrix dimension error\n");
		 return -1;
	 }
	 int memoryArrayValues=sizeof(double)*numTasks*numResources;
	 pa->values=(double*)malloc(memoryArrayValues);

	 int memoryArrayResources=sizeof(Resource)*numResources;
	 pa->resources=(Resource*)malloc(memoryArrayResources);

	 int memoryArrayTasks=sizeof(Task)*numTasks;
	 pa->tasks=(Task*)malloc(memoryArrayTasks);
	 pa->numTask=numTasks;
	 pa->numResources=numResources;
	 for (int i=0;i<numTasks;i++){
	 			pa->tasks[i]=tasks[i];

	 }
	 for (int i=0;i<numResources;i++){
	 			pa->resources[i]=resources[i];

	 }
	 double aux;
	 int numValues=numTasks*numResources;

	 for (int i=0;i<numValues;i++){

	 			aux=values[i];
	 			pa->values[i]=aux;

	 }
	 pa->type=MAX;//default//TODO
	 return 0;
 }
 int deleteAProblem(PAproblem pa){//free memory
	 int res=0;
	 free(pa->values);
	 free(pa->tasks);
	 free(pa->resources);
	 return res;
 }
 void showAproblem(PAproblem pap){
	 printf("inside showAproblem\n");
	 int m=pap->numTask;
	 int j=0;
	 for(int i=0;i<m;i++){
		 printf("%s ",pap->tasks[i].name);
	 }
	 int n=pap->numResources;

	 printf("\n%s ",pap->resources[j].name);
	 for(int i=0;i<m*n;i++){
		 double r=(i+1)%m;
		 if(r!=0.){
			 if(i>0 &&  i%m==0){
				printf("%s ",pap->resources[j+1].name);
				printf("%f ",pap->values[i]);
				j++;

			 }else{

				printf("%f ",pap->values[i]);
			 }
		 }
		 else{
				 printf("%f \n",pap->values[i]);
		 }

	 }

 }
 void showAProblems(const PAproblem pap, int nGP){
 	int i;
 	for(i=0;i<nGP;i++){
 		printf("Problem %d:",i+1);
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
	 Task arrayT[numTasks];
	 Resource arrayR[numResources];
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
			 init_task(&task,c);
			 arrayT[j]=task;
		 }
		 //resources
		 for(j=0;j<numResources;j++){
			 fgets(c,TAM_MAX_READ,f);
			 quitaSaltoDeLinea(c);
			 init_resource(&resource,c);
			 arrayR[j]=resource;
		 }
		 //values

		 for(i=0;i<numValues;i++){
			 fgets(c,TAM_MAX_READ,f);
			 quitaSaltoDeLinea(c);
			 aux=atof(c);
			 values[i]=aux;
		 }
		 res=initAProblem(pap,arrayT,arrayR,numTasks,numResources, values);

	 }
	 fclose(f);
	 return res;
 }
 Type getAproblemType(PAproblem pap){
	 Type res;
	 res=pap->type;
	 return res;
 }

 int get_max_num_problems(PAproblem pa){

	 int numTasks=pa->numTask;
	 for(int i=0;i<pa->numTask-1;i++){
		 numTasks=numTasks*(pa->numTask-i-1);
	 }
	 return numTasks;
 }

 int get_max_num_alternatives(PAproblem pap){
	 int res=0;
	 res=pap->numResources;
	 return res;
 }
 ////////////////////////////CHECKERS////////////////////////////////////////
 Logico checkTasks(PTask tasks){
	  Logico res=FALSE;
	  if(tasks!=NULL){
		  res=TRUE;
	  }
	  return res;
 }
 Logico checkResources(PResource resources){
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



