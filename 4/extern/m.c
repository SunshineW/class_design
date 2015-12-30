#include<stdlib.h>

#include"m.h"
#include"private.h"
#include"a.h"
#include"b.h"

struct Graph_Spec *Specs[] = {&A_Spec, &B_Spec};

struct Graph *Make(enum Type T)
{
	struct Graph *G;
	G = malloc(sizeof(struct Graph));
	G->Private = Specs[T];
	(*G->Private->Make)(G);
	return G;
}
int Print(struct Graph * G)
{
	(*G->Private->Print)(G);
	return 0;
}


