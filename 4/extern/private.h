#ifndef P_H_
#define P_H_

#include "m.h"

struct Graph_Spec
{
	int (*Make) (struct Graph *);
	int (*Print) (struct Graph *);
};

#endif