#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<malloc.h>

#include "sllist.h"

int SLInit(SLList **L)
{
	int result = SL_SUCCESS;
	assert(L != NULL && (*L) == NULL);

	*L = (SLList*)malloc(sizeof(SLList));
	if (*L != NULL)
	{
		(*L)->object = NULL;
		(*L)->next = NULL;
	}
	else
	{
		result = SL_NO_MEM;
	}
	return result;
}

int SLAdd(SLList *L, int tag, void *object, size_t size)
{
	SLList *newItem = NULL;
	int result = SL_SUCCESS;
	assert(L != NULL);
	if (size > 0)
	{
		newItem = (SLList *)malloc(sizeof(SLList));
		if (newItem != NULL)
		{
			newItem->tag = tag;
			newItem->size = size;
			newItem->object = malloc(size);

			if (newItem->object != NULL)
			{
				/* Inset after L */
				memcpy(newItem->object, object, size);
				newItem->next = L->next;
				L->next = newItem;
				result = SL_SUCCESS;
			}
			else
			{
				free(newItem);
				result = SL_NO_MEM;
			}
		}
		else
		{
			result = SL_NO_MEM;
		}
	}
	else
	{
		result = SL_ZERO_SIZE;
	}
	return result;
}

int SLAppend(SLList *L, int tag, void *object, size_t size)
{
	int result = SL_SUCCESS;
	SLList *endSeeker = L;
	while (endSeeker->next != NULL)
	{
		endSeeker = endSeeker->next;
	}
	result = SLAdd(endSeeker, tag, object, size);
	return result;
}

int SLUpdate(SLList *item, int newTag, void *newObject, size_t newSize)
{
	int result = SL_SUCCESS;
	void *p = NULL;
	if (newSize > 0)
	{
		p = realloc(item->object, newSize);
		if (p != NULL)
		{
			item->object = p;
			memmove(item->object, newObject, newSize);
			item->tag = newTag;
			item->size = newSize;

		}
		else
		{
			result = SL_NO_MEM;
		}
	}
	else
	{
		result = SL_ZERO_SIZE;
	}
	return result;
}

void * SLGetData(SLList *item, int *tag, size_t *size)
{
	void *p = NULL;
	if (item != NULL)
	{
		if (tag != NULL)
		{
			*tag = item->tag;
		}
		if (size != NULL)
		{
			*size = item->size;
		}
		p = item->object;
	}
	return p;
}

SLList *SLDeleteThis(SLList *item)
{
	SLList *nextNode = NULL;
	if (item != NULL)
	{
		nextNode = item->next;
		if (item->object != NULL)
		{
			free(item->object);
		}
		free(item);
	}
	return nextNode;
}

void SLDeleteNext(SLList *item)
{
	if (item != NULL && item->next != NULL)
	{
		item->next = SLDeleteThis(item->next);
	}
}


void SLDestory(SLList *L)
{
	SLList *next = L;
	do
	{
		next = SLDeleteThis(next);
	} while (next != NULL);
	L = NULL;
}

SLList *SLWalk(SLList *L, int(*Func)(int, void *, void *), void *args)
{
	SLList *thisItem;
	int result = 0;
	for (thisItem = L->next; thisItem != NULL; thisItem = thisItem->next)
	{
		result = (*Func)(thisItem->tag, thisItem->object, args);
		if (result != 0)
		{
			break;
		}
	}
	return thisItem;
}

SLList *SLGetPri(SLList *L, SLList *node)
{
	SLList *head = L;
	while (head->next != node)
	{
		head = head->next;
	}
	return head;
}

