#ifndef SLLIST_H_
#define SLLIST_H_

#define SL_SUCCESS 0
#define SL_NO_MEM -1
#define SL_ZERO_SIZE -2

typedef struct SLList
{
	int tag;
	struct SLList *next;
	void *object;
	size_t size;
}SLList;

int SLInit(SLList **L);

/* Add new item immediately after current item */
int SLAdd(SLList *L, int tag, void *object, size_t size);

/* Add new item right at the end of the list */
int SLAppend(SLList *L, int tag, void *object, size_t size);

/* Replace existing data */
int SLUpdate(SLList *item, int newTag, void *newObject, size_t newSize);

/* Retrieve data from this node */
void * SLGetData(SLList *item, int *tag, size_t *size);

/* Delete this item. Returns pointer to next item 
 * caller's responsibility to maintain list integrity 
 */
SLList *SLDeleteThis(SLList *item);

/* Delete item immediately following the one passed in.
 * List integrity maintained automatically.
 */
void SLDeleteNext(SLList *item);

/* Destory the entire list */
void SLDestory(SLList *L);

/* Call func(Tag, ThisItem, Args) for each item 
 * return pointtor to current item when func return -1
 * else return null.
 */
SLList *SLWalk(SLList *L, int(*Func)(int, void *, void *), void *args);

/* return the pointer to whom it's next_pointor point the node*/
SLList *SLGetPri(SLList *L, SLList *node);

#endif
