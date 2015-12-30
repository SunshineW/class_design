#ifndef STACK_H__
#define STACK_H__

#include "sllist.h"

#define STACK_SUCCESS       0
#define STACK_PUSH_FAILURE  1
#define STACK_POP_FAILURE   2
#define STACK_EMPTY         3

typedef struct STACK
{
#ifndef NDEBUG
  int CheckInit1;
#endif

  SLLIST *StackPtr;
  size_t NumItems;

#ifndef NDEBUG
  int CheckInit2;
#endif
} STACK;

int StackPush(STACK *Stack,
              int Tag,
              void *Object,
              size_t Size);
int StackPop(void *Object, STACK *Stack);

void *StackGetData(STACK *Stack, int *Tag, size_t *Size);

size_t StackCount(STACK *Stack);

void StackDestroy(STACK *Stack);


#endif
