#include<stdlib.h>
#include<stdio.h>

#include"a.h"

int A_Make(struct Graph *G)
{
	G->id = 1;
	printf("A_Make\n");
	return 0;
}

int A_Print(struct Graph *G)
{
	printf("A_Print: %d\n", G->id);
	return 0;
}

struct Graph_Spec A_Spec=
{
	A_Make,
	A_Print
};


