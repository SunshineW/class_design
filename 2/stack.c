#include <string.h>
#include <assert.h>

#include "stack.h"

int StackPush(STACK *Stack,
              int Tag,
              void *Object,
              size_t Size)
{
  int Result = STACK_PUSH_FAILURE;
  int ListResult;
  assert(Stack != NULL);
  assert(0 == Stack->CheckInit1 && 0 == Stack->CheckInit2);

  ListResult = SLFront(&Stack->StackPtr, Tag, Object, Size);

  if(SL_SUCCESS == ListResult)
  {
    Result = STACK_SUCCESS;
    ++Stack->NumItems;
  }

  return Result;
}

int StackPop(void *Object, STACK *Stack)
{
  size_t Size;
  void *p;
  int Result = STACK_SUCCESS;

  assert(Stack != NULL);
  assert(0 == Stack->CheckInit1 && 0 == Stack->CheckInit2);

  if(Stack->NumItems > 0)
  {
    p = SLGetData(Stack->StackPtr, NULL, &Size);
    if(p != NULL)
    {
      if(Object != NULL)
      {
        memcpy(Object, p, Size);
      }
    }
    else
    {
      Result = STACK_POP_FAILURE;
    }
    Stack->StackPtr = SLDeleteThis(Stack->StackPtr);
    --Stack->NumItems;
  }
  else
  {
    Result = STACK_EMPTY;
  }

  return Result;
}

void *StackGetData(STACK *Stack, int *Tag, size_t *Size)
{
  assert(Stack != NULL);
  assert(0 == Stack->CheckInit1 && 0 == Stack->CheckInit2);

  return SLGetData(Stack->StackPtr, Tag, Size);
}

size_t StackCount(STACK *Stack)
{
  assert(Stack != NULL);
  assert(0 == Stack->CheckInit1 && 0 == Stack->CheckInit2);
  return Stack->NumItems;
}

void StackDestroy(STACK *Stack)
{
  assert(Stack != NULL);
  assert(0 == Stack->CheckInit1 && 0 == Stack->CheckInit2);

  while(StackCount(Stack) > 0)
  {
    StackPop(NULL, Stack);
  }
  Stack->StackPtr = NULL;
}
