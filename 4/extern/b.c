#include<stdio.h>
#include<stdlib.h>
#include"b.h"
int B_Make(struct Graph *G)
{
	G->id = 2;
	printf("B_make\n");
	return 0;
}

int B_Print(struct Graph *G)
{
	printf("B_Print: %d\n", G->id);
	return 0;
}
struct Graph_Spec B_Spec=
{
	B_Make,
	B_Print
};


