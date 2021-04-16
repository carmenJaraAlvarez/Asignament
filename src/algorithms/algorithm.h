/*
 * algorithm.h
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */

#ifndef ALGORITHM_ALGORITHM_H_
#define ALGORITHM_ALGORITHM_H_


//Redistribution
typedef struct
  {

	int *transfered;
	int len_transfered;

  } Transfered_nodes;

  typedef Transfered_nodes* PTransfered_nodes;


  int init_transfered(PTransfered_nodes);
  int delete_transfered(PTransfered_nodes);//free memory


#endif /* ALGORITHM_ALGORITHM_H_ */
