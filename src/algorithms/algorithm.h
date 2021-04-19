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

//Redistribution////////////////////////////////////
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
  void show_transfered(const Transfered_nodes *);

  //pair prune ////////////////////////////////////

  typedef struct
  {
	  int * solution_tuples;
	  double * solution_values;
	  int num_tuples;

  }Tuple_prune;
  typedef Tuple_prune* PTuple_prune;

  int init_tuple_prune(PTuple_prune);
  int copy_tuple_prune(PTuple_prune,const PTuple_prune);
  int add_tuple(PTuple_prune, int, int, double);
  int free_tuple_prune(PTuple_prune);
  void show_tuple_prune(const PTuple_prune);


#endif /* ALGORITHM_ALGORITHM_H_ */
