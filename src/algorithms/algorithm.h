/*
 * algorithm.h
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */

#ifndef ALGORITHM_ALGORITHM_H_
#define ALGORITHM_ALGORITHM_H_
#include "../assignment/PD/a_problem_PD.h"
#define MAX_NUM 100

//Redistribution
typedef struct
  {

	AproblemPD *transfered;
	int *receivers;
	int len_transfered;

  } Transfered_nodes;

  typedef Transfered_nodes* PTransfered_nodes;


  int init_transfered(PTransfered_nodes);
  int free_transfered(PTransfered_nodes);//free memory
  int copy_transfered(Transfered_nodes *,const Transfered_nodes *);
  int delete_transfered(Transfered_nodes *,int);
  int add_transfered(Transfered_nodes * ,AproblemPD * ,int );
  void show_transfered(Transfered_nodes *);

#endif /* ALGORITHM_ALGORITHM_H_ */
