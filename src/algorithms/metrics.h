/*
 * metrics.h
 *
 *  Created on: 21 dic. 2020
 *      Author: practica
 */

#ifndef ALGORITHM_METRICS_H_
#define ALGORITHM_METRICS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include "mpe.h"

extern double startwtime, endwtime;

extern int event1a;
extern int event1b;
extern int event2a;
extern int event2b;
extern int event3a;
extern int event3b;
extern int event4a;
extern int event4b;
extern int event5a;
extern int event5b;
extern int event6a;
extern int event6b;
extern int event1, event2, event3, event4, event5, event6, event7;
extern int startEvent, endEvent;

int init_clock();
double end_clock();
int describe_logs();


#endif /* ALGORITHM_METRICS_H_ */
