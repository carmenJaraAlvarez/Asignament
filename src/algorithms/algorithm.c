/*
 * algorithm.c
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */

#include "algorithm.h"

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
	  if(1)
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
	  if(1)
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
  	if(1)
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
  void show_transfered(Transfered_nodes * t)
  {
	  printf("\n%d transfered nodes:\n",t->len_transfered);
	  for(int i=0;i<t->len_transfered;i++)
	  {
		  printf("\nsend to %d",t->receivers[i]);
		  show_aproblem_PD(&(t->transfered[i]));
;
	  }

  }
