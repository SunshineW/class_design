#ifndef MYHEAD_H_
#define MYHEAD_H_

#include "lib/sllist.h"

typedef struct Term
{
	int c;  /*系数*/
	int e;  /*指数*/
}Term;

int Find(int tag, void *memory, void *args);

int PrintItem(int tag, void *memory, void *args);

int FindInsertPosition(int tag, void *memory, void *args);

void pause();

void AddTerm(SLList *L);

void Combine(SLList *L1, SLList *L2, int commond);

void menu();

#endif