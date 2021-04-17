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
	  int memoryArrayValues=sizeof(int)*MAX_NUM;
	  pt->transfered=(int*)malloc(memoryArrayValues);
	  pt->receivers=(int*)malloc(memoryArrayValues);
	  return 0;
  }
  int free_transfered(PTransfered_nodes pt)
  {
	  free(pt->transfered);
	  return 0;
  }
  int copy_transfered(PTransfered_nodes dest,const PTransfered_nodes origin)
  {
	  dest->len_transfered=origin->len_transfered;
	  for(int i=0;i<dest->len_transfered;i++)
	  {
		  dest->receivers[i]=origin->receivers[i];
		  dest->transfered[i]=origin->transfered[i];
	  }
	  return 0;
  }
