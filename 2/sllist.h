#ifndef SLLIST_H__
#define SLLIST_H__

#define SL_SUCCESS    0
#define SL_NO_MEM     1
#define SL_ZERO_SIZE  2


typedef struct SLLIST
{
  int Tag;
  struct SLLIST *Next;
  void *Object;
  size_t Size;
} SLLIST;

/* Add new item immediately after current item */
int SLAdd(SLLIST **Item,
          int Tag,
          void *Object,
          size_t Size);

/* Add item to front of list. Care: if you pass
 * this function any node other than the first,
 * you will get Y-branches in your list:
 *
 *    oldroot-->-
 *               \  
 *                >->-passeditem-->-->--oldnext
 *               /
 *    newitem-->-
 *
 * This would be a Bad Thing.
 */
int SLFront(SLLIST **Item,
            int Tag,
            void *Object,
            size_t Size);

/* Add new item right at the end of the list */
int SLAppend(SLLIST **Item,
             int Tag,
             void *Object,
             size_t Size);

/* Replace existing data */
int SLUpdate(SLLIST *Item,
             int NewTag,
             void *NewObject,
             size_t NewSize);

/* Retrieve data from this node */
void *SLGetData(SLLIST *Item,
                int *Tag,
                size_t *Size);

/* Delete this item. Returns pointer to
 * next item - caller's responsibility
 * to maintain list integrity.
 */
SLLIST *SLDeleteThis(SLLIST *Item);

/* Delete item immediately following
 * the one passed in. List integrity
 * maintained automatically.
 */
void SLDeleteNext(SLLIST *Item);

/* Destroy the entire list */
void SLDestroy(SLLIST **List);

/* Call func(Tag, ThisItem, Args) for each item */
int SLWalk(SLLIST *List,
           int(*Func)(int, void *, void *),
           void *Args);
#endif
