/*
 * problem_MPI.h
 *
 *  Created on: 13 ene. 2021
 *      Author: practica
 */

#ifndef MPI_PROBLEM_MPI_H_
#define MPI_PROBLEM_MPI_H_
#include "mpi.h"
#include "mpe.h"
#include "mpe_log.h"
#include "../PD/PD_algorithm.h"

#define MAX_NUM_ALT	100
#define MAX_TAM_STRING	1000
#define TIMEOUT	1000000

#define tag_work 0
#define tag_values 1
#define tag_tasks 2
#define tag_resources 3
#define tag_alternatives 4
#define tag_best 5
#define tag_ask_work 6
#define tag_give_work 7
#define tag_redistribution 1000//init rank-> tag_redistribution+ id receiver
#define tag_resolved 100

extern int redistribution_rr;
extern int print_all;
extern int numprocs;

extern MPI_Comm world;
extern MPI_Comm slaves;

extern int event1a;
extern int event1b;
extern int event2a;
extern int event2b;
extern int event3a;
extern int event3b;
extern int event4a, event4b;
extern int event5a, event5b;
extern int event6a, event6b;
extern int event7a, event7b;
extern int event8a,event8b;
extern int event1, event2, event3, event4, event5, event6, event7, event8;
extern int startEvent, endEvent;

extern MPI_Request request_bcast;
extern MPI_Request request_b;
extern double best;
extern int tuple_p;
extern int fs;
extern int type_best;
extern int master;
extern int numprocs;
AlgorithmPD final_alg;
int n;//buffer for ask work
double b;//buffer for best
double new_best;//buffer for broadcast best
int final_sol[100];//TODO test

AproblemPD *newArrayAppd;
AproblemPD *problems;



  struct  Work
	  {
		int num_tasks;
		int num_resources;
		//problem type: min, max, other
		Type type;
		int prune;
		int tuple_prune;
		int redistribution;
		int first_search;
		//alternative the process will select
		int num_alternatives;
		double best;

	  }w_mpi;

  typedef struct
	  {
		int index;
		double value;
		int solution[100];//TODO
	  } Node_work;

  typedef struct
	  {
		int num_resolved;
		double value;
		int resources[100];//TODO


	  }Resolved;
  typedef Resolved* PResolved;

  typedef struct
	  {
		Aproblem ap;
		int num_processes;

	  }Problem_MPI;
  typedef Problem_MPI* Pproblem_MPI;

  typedef struct
	  {
		int index;
		int * receivers_plus;
		int len_receivers_plus;
		int * receivers_less;
		int len_receivers_less;
		int * receivers_empty;
		int len_receivers_empty;
	  }Distribution_rr;


//slaves
int rcv_work(double *,MPI_Request * ,int *);
int ask_work();
int init_work(PAproblem, int, int*,double *,MPI_Request *,int *, double);
int send_resolved(const PalgorithmPD);
int send_best(const PalgorithmPD);
int init_waiting_best(double *,MPI_Request*);
void waiting_best(double *,MPI_Request*);
int log_prune();
int sending_my_work(int,PalgorithmPD,int);
int waiting_petition(int * , MPI_Request* ,PalgorithmPD ,int, int*);
int confirming_work(int);
int waiting_confirming(Transfered_nodes *);

//master
int distribution(PalgorithmPD, int, int,int,int);
int send_work(const PalgorithmPD,int *, int, int, int,int,int,int,double);
int broadcast_best(const double);
int rcv_resolved();
int rcv_best(double b, MPI_Request*);
int scan_petition(MPI_Request*, MPI_Request*,MPI_Request*);
int init_listening(MPI_Request*, MPI_Request*);
int finish_work();
int give_me_work(int process);
int resolved();
void init_best(MPI_Request *, MPI_Comm *);
int init_redistribution(MPI_Request * );

//aux
int serializer_tasks(PalgorithmPD, char*);
int deserializer_tasks(char*, int, PTask );
int serializer_resources(PalgorithmPD palg, char* all);
int deserializer_resources(char*, int, PResource);



#endif /* MPI_PROBLEM_MPI_H_ */
