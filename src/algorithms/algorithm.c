/*
 * algorithm.c
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */

#include "algorithm.h"
double best_in_row(const PAproblem,int, int *);
int is_in_array(int ,int, int *);


  int init_transfered(PTransfered_nodes pt)
  {
	  pt->len_transfered=0;
	  int memoryArrayValues=sizeof(AproblemPD)*MAX_NUM;
	  pt->transfered=(AproblemPD*)malloc(memoryArrayValues);
	  memoryArrayValues=sizeof(int)*MAX_NUM;
	  pt->receivers=(int*)malloc(memoryArrayValues);
	  return 0;
  }
  int free_transfered(PTransfered_nodes pt)
  {
	  free(&(pt->transfered));
	  return 0;
  }
  int copy_transfered(Transfered_nodes * dest,const Transfered_nodes * origin)
  {
	  if(print_all)
	  {
		  printf("\nalgorithm.c		copy_transfered  origin and dest");
		  show_transfered(origin);
		  show_transfered(dest);
	  }
	  dest->len_transfered=origin->len_transfered;
	  for(int i=0;i<dest->len_transfered;i++)
	  {
		  dest->receivers[i]=origin->receivers[i];
		  copy_aproblem_PD(&(dest->transfered[i]),origin->transfered[i]);

	  }
	  if(print_all)
	  {
		  printf("\nalgorithm.c		copy_transfered  origin and dest END");
		  show_transfered(origin);
		  show_transfered(dest);
	  }
	  return 0;
  }
  int delete_transfered(Transfered_nodes * transf,int rcvr)
  {
  	Transfered_nodes aux;
  	init_transfered(&aux);
  	int j=0;
  	for(int i=0;i<transf->len_transfered;i++)
  	{
  		if(transf->receivers[i]!=rcvr)
  		{
  			aux.len_transfered++;
  			aux.receivers[j]=transf->receivers[i];
  		    copy_aproblem_PD(&(aux.transfered[j]),transf->transfered[i]);
  			j++;
  		}
  	}
  	if(print_all)
  	{
  		printf("\nalgorithm.c		delete_transfered %d",rcvr);
  		show_transfered(&aux);
  	}
  	copy_transfered(transf,&aux);
  	return 0;
  }

  int add_transfered(Transfered_nodes * transf,AproblemPD * a,int rcvr)
  {
	  copy_aproblem_PD(&(transf->transfered[transf->len_transfered]),*a);
	  transf->receivers[transf->len_transfered]=rcvr;
	  transf->len_transfered++;

	  return 0;
  }
  void show_transfered(const Transfered_nodes * t)
  {
	  printf("\n%d transfered nodes:\n",t->len_transfered);
	  for(int i=0;i<t->len_transfered;i++)
	  {
		  printf("\nsend to %d",t->receivers[i]);
		  show_aproblem_PD(&(t->transfered[i]));
	  }

  }
  int init_tuple_prune(PTuple_prune ptp){
	  ptp->num_tuples=0;
	  int memoryArrayValues=sizeof(int)*MAX_NUM;//TODO it must depend of tasks number
	  ptp->solution_values=(int*)malloc(memoryArrayValues);
	  memoryArrayValues*=2;
	  ptp->solution_tuples=(int*)malloc(memoryArrayValues);

	return 0;
  }
  int copy_tuple_prune(PTuple_prune dest,const PTuple_prune origin){
	  for(int i=0;i<origin->num_tuples;i++)
	  {
		  dest->solution_values[i]=origin->solution_values[i];
		  dest->solution_tuples[i*2]=origin->solution_tuples[i*2];
		  dest->solution_tuples[i*2+1]=origin->solution_tuples[i*2+1];
	  }
	  dest->num_tuples=origin->num_tuples;
	  return 0;
  }
  int add_tuple(PTuple_prune ptp, int t1, int t2, double acum){
	  ptp->solution_values[ptp->num_tuples]=acum;
	  ptp->solution_tuples[ptp->num_tuples*2]=t1;
	  ptp->solution_tuples[ptp->num_tuples*2+1]=t2;
	  ptp->num_tuples++;
	  return 0;
  }
  int free_tuple_prune(PTuple_prune ptp)
  {
	  free(ptp->solution_tuples);
	  free(ptp->solution_values);
	  return 0;
  }
  void show_tuple_prune(const PTuple_prune ptp)
  {
	  printf("\nTuple prune:\n{");
	  for(int i=0;i<ptp->num_tuples;i++)
	  {
		  printf("{");
		  printf("(%d,%d) %f",ptp->solution_tuples[i*2],ptp->solution_tuples[i*2+1],ptp->solution_values[i]);
		  printf("}");
	  }
	  printf("}");
  }

  void check_prune(
		  Logico * tuple_prune,
		  PTuple_prune tuple_prune_data,
		  Alternative * as,
		  AproblemPD * papd,
		  double my_value,
		  int u,
		  Logico ismin,
		  Logico ismax)
  {
	  if(tuple_p)
	  {
		  //tuple_prune empty, save data sol
		  	  if(tuple_prune_data->num_tuples==0)
		  	  {
		  		  printf("\nalgorithm.c		check_prune()	adding new: %d,%d->%f",papd->solution.resources[papd->solution.lengthArrays-1].position,as[u].indexResource,papd->solution.acum+my_value);
		  			  add_tuple(
		  					  tuple_prune_data,
		  					  papd->solution.resources[papd->solution.lengthArrays-1].position,
		  					  as[u].indexResource,
		  					  papd->solution.acum+my_value);
		  			  show_tuple_prune(tuple_prune_data);
		  	  }
		  	  else //si second round and tuple_prune NOT empty, compare
		  	  {
		  		  if(print_all)
		  		  {
		  			printf("\nalgorithm.c		check_prune()	search");
		  		  }

		  		  //search
		  		  int sym=-1;
		  		  for(int j=0;j<tuple_prune_data->num_tuples;j=j+2)
		  		  {
		  			  if(tuple_prune_data->solution_tuples[j]==as[u].indexResource
		  					  &&
		  				 tuple_prune_data->solution_tuples[j+1]==papd->solution.resources[papd->solution.lengthArrays-1].position  )
		  			  {
		  				  sym=j/2;
		  				  show_tuple_prune(tuple_prune_data);
		  				  if(print_all)
		  				  {
		  					printf("\nalgorithm.c		check_prune()	tuple found: %d",sym);
		  				  }
		  				  break;
		  			  }
		  		  }
		  		  my_value=my_value+papd->solution.acum;
		  		  //if exists symmetric and acum is better>>prune
		  		  if(
		  				  sym>=0
		  				  &&
		  				  (
		  						  (tuple_prune_data->solution_values[sym]>my_value && ismax)
		  						  ||
		  						  (tuple_prune_data->solution_values[sym]<my_value && ismin)
		  						  )
		  				  )
		  		  {
		  			  *tuple_prune=TRUE;
		  			  MPE_Log_event(event9, 0, "Tuple prune");
		  			  if(print_all)
		  			  {
		  				 printf("\nalgorithm.c		check_prune()	tuple_prune=true");
		  			  }

		  		  }
		  		  else//save and prune=FALSE to go on
		  		  {
		  			  add_tuple(
		  						  tuple_prune_data,
		  						  papd->solution.resources[papd->solution.lengthArrays-1].position,
		  						  as[u].indexResource,
		  						  my_value);
		  			  if(print_all)
		  			  {
		  				show_tuple_prune(tuple_prune_data);
		  			  }

		  			  *tuple_prune=FALSE;
		  		  }


		  	  }

	  }

  }
  double best_diagonal(const PAproblem pap)
  {
	  double res=0.;
	  double second=0.;
	  for(int i=0;i<pap->numTask;i++)
	  {
		  res=res+pap->values[(i*pap->numTask)+i];
		  if(print_all)
		  {
			  printf("\nalgorithm.c		best_diagonal()		loop %d first diagonal: %f",i,res);
		  }
	  }
	  for(int i=0;i<pap->numTask;i++)
	  {
		  second=second+pap->values[pap->numTask*(i+1)-1-i];
		  if(print_all)
		  {
			  printf("\nalgorithm.c		best_diagonal()		loop %d second diagonal: %f",i,second);
		  }
	  }
	  if(((second>res) && (pap->type==MAX))
			  ||
			  (second<res && pap->type==MIN))
	  {
		  res=second;
	  }
	  if(print_all)
	  {
		  printf("\nalgorithm.c		best_diagonal()		%f",res);
	  }
	  return res;

  }
  double greedy(const PAproblem pap)
  {
	  double res=0.;
	  int array_sol[pap->numTask];

	  for(int i=0;i<pap->numResources;i++)
	  {
		  res=res+best_in_row(pap,i,&array_sol);
		  if(print_all)
		  {
			  printf("\nalgorithm.c		greedy()		best acum row %d: %f",i,res);
		  }
	  }
	  if(print_all)
	  {
		  printf("\nalgorithm.c		greedy()		%f",res);
	  }

	  return res;
  }
  int is_in_array(int i,int row, int * array_sol)
  {
	  int res=0;//true
	  for(int j=0;j<row;j++)
	  {
		  if(i==array_sol[j])
		  {
			  res=1;//false

			  break;
		  }
	  }
	  return res;
  }
  double best_in_row(const PAproblem pap,int row, int * array_sol)
  {
	  if(print_all)
	  {
		 printf("\nalgorithm.c		best_in_row() %d",row);
	  }
	  double res;
	  double aux;
	  int isfirst=1;
	  for(int i=0;i<pap->numTask;i++)
	  {
		  if(print_all)
		  {
			 printf("\nalgorithm.c		best_in_row() Column %d",i);
		  }

		  if(!is_in_array(i,row, array_sol))
		  {
			  aux=pap->values[row*pap->numTask+i];
			  //if is first value, we take it
			  if(isfirst)
			  {
				  res=aux;
				  array_sol[row]=i;
				  isfirst=0;
			  }
			  else
			  {
				  if((pap->type==MAX && aux>res )
				  					  ||
				  					  (pap->type==MIN && aux<res))
				  {
					  if(print_all)
					  {
						 printf("\nalgorithm.c		best_in_row() change to res to %f",aux);
					  }
					  res=aux;
					  array_sol[row]=i;
				  }//end if is better
			  }//end is not first
		  }	//end is not in array
	  }//end for

	  if(print_all)
	  {
		  for(int a=0;a<row;a++)
		  {
			  printf("\nalgorithm.c		best_in_row()  array[%d]=%d",a,array_sol[a]);
		  }
	  }

	  return res;
  }



