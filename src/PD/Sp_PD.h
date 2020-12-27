
#ifndef PD_SP_PD_H_
#define PD_SP_PD_H_
#include "../assignment/alternative.h"
#define TAM_MAX_SP 100
typedef struct
  {
	Alternative alternative;
	double value;

  } SpPD;
  typedef SpPD* PSpPD;
  typedef SpPD ArraySpPD[TAM_MAX_SP];

#endif /* PD_SP_PD_H_ */
