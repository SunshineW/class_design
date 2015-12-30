#ifndef QUEUE_H__
#define QUEUE_H__

#include "sllist.h"

#define QUEUE_SUCCESS       0
#define QUEUE_ADD_FAILURE   1
#define QUEUE_DEL_FAILURE   2
#define QUEUE_EMPTY         3

typedef struct QUEUE
{
#ifndef NDEBUG
  int CheckInit1;
#endif

  SLLIST *HeadPtr;
  SLLIST *TailPtr;
  size_t NumItems;

#ifndef NDEBUG
  int CheckInit2;
#endif
} QUEUE;

int QueueAdd(QUEUE *Queue,
             int Tag,
             void *Object,
             size_t Size);
int QueueRemove(void *Object, QUEUE *Queue);

void *QueueGetData(QUEUE *Queue, int *Tag, size_t *Size);
size_t QueueCount(QUEUE *Queue);

void QueueDestroy(QUEUE *Queue);

#endif
