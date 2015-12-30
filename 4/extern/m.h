#ifndef M_H_
#define M_H_

#include<limits.h>

enum Type {A, B};

struct Graph
{
	int id;
	struct Graph_Spec *Private;
};
struct Graph * Make(enum Type T);

int Print(struct Graph * G);

#endif