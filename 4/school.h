#ifndef SCHOOL_H_
#define SCHOOL_H_

#include "graphs.h"

static int scenery_id = 100;

struct Scenery
{
	int id;
	char name[32];
	char summary[256];
};

int SearchScenery(struct Graph *G);

int AddScenery(struct Graph *G);

int ShowAllScen(struct Graph *G);

int AddPath(struct Graph *G);

int UpdataScenery(struct Graph *G);

int MinPath(struct  Graph *G);

int FreeScenery(struct Graph *G);

int Scenery_init(struct Graph *G);

#endif