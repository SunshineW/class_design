#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<malloc.h>

#include"myhead.h"

int Find(int tag, void *memory, void *args)
{
	Term *thisTerm = memory;
	Term *findTerm = args;
	if (thisTerm->e == findTerm->e)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int PrintItem(int tag, void *memory, void *args)
{
	Term *thisTerm = memory;
	if (thisTerm->c == 1)
	{
		printf("x");
	}
	else if(thisTerm->c == -1)
	{
		printf("-x");
	}
	else
	{
		printf("%dx", thisTerm->c);
	}
	
	if (thisTerm->e != 1)
	{
		printf("^%d", thisTerm->e);
	}
	printf("  ");
	return 0;
}

int FindInsertPosition(int tag, void *memory, void *args)
{
	Term *thisTerm = memory;
	Term *insertTerm = args;
	if (thisTerm->e < insertTerm->e)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void pause()
{
	printf("\nPress any key to continue...");
	setbuf(stdin, NULL);
	getchar();
}

void AddTerm(SLList *L)
{
	Term newTerm = { 0 };
	printf("请输入新项的系数: ");
	scanf("%d", &(newTerm.c));
	printf("请输入新项的指数: ");
	scanf("%d", &(newTerm.e));
	if (newTerm.c != 0)
	{
		SLList *find = SLWalk(L, Find, &newTerm);
		if (find == NULL)  /* 该项不存在链表中, 把该项插入到链表中, 并保持链表有序  */
		{
			SLList *insertPos = SLWalk(L, FindInsertPosition, &newTerm); /* 寻找插入位置 */
			if (insertPos == NULL)
			{
				SLAppend(L, 0, &newTerm, sizeof(Term));
			}
			else
			{
				SLAdd(SLGetPri(L, insertPos), 0, &newTerm, sizeof(Term));
			}

		}
		else  /* 该项已存在于链表中, 则合并此项 */
		{
			Term *oldTerm = SLGetData(find, NULL, NULL);
			newTerm.c += oldTerm->c;
			if (newTerm.c == 0)  /* 如果合并后 系数为0, 则删除该项 */
			{
				SLDeleteNext(SLGetPri(L, find));
			}
			else
			{
				SLUpdate(find, 0, &newTerm, sizeof(Term));
			}
		}
	}
	printf("添加成功\n");
}

void Combine(SLList *L1, SLList *L2, int commond) /* commond: 1表示相加, 2表示相减 */
{
	SLList *L = NULL;
	SLInit(&L);
	SLList *head = L1;
	while (head->next != NULL)
	{
		head = head->next;
		SLAppend(L, head->tag, head->object, head->size);
	}
	head = L2;

	while (head->next != NULL)
	{
		head = head->next;
		SLList *find = SLWalk(L, Find, head->object);
		if (find == NULL) /* 链表中没有此项 */
		{
			SLList *insertPos = SLWalk(L, FindInsertPosition, head->object);
			if (insertPos == NULL) /* 挂在链尾 */
			{
				SLAppend(L, head->tag, head->object, head->size);
			}
			else  /* instet before insertPos */
			{
				SLAdd(SLGetPri(L, insertPos), head->tag, head->object, head->size);
			}
		}
		else  /*链表中已存在此项 */
		{
			Term *oldTerm = find->object;
			Term *newTerm = head->object;
			if (commond == 1)
			{
				oldTerm->c += newTerm->c;
			}
			else if (commond == 2)
			{
				oldTerm->c -= newTerm->c;
			}

			if (oldTerm->c == 0)
			{
				SLDeleteNext(SLGetPri(L, find));
			}
			else
			{
				SLUpdate(L, 0, oldTerm, sizeof(Term));
			}
		}
	}
	SLWalk(L, PrintItem, NULL);
	SLDestory(L);
}

void menu()
{
	printf("\n");
	printf("      1.显示多项式A\n");
	printf("      2.显示多项式B\n");
	printf("      3.计算A+B\n");
	printf("      4.计算A-B\n");
	printf("      5.为多项式A添加新项\n");
	printf("      6.为多项式B添加新项\n");
	printf("      0.退出\n");
	printf("\n");
	printf("请选择: ");
}