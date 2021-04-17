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
  int delete_transfered(PTransfered_nodes pt)
  {
	  free(pt->transfered);
	  return 0;
  }
