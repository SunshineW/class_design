#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"myhead.h"

int main()
{
	int i;
	Term a[3] = { { 1,3 },{ 1,2 },{ 1,1 } };
	Term b[3] = { { 1,3 },{ 1,2 },{ -1,1 } };
	SLList *L1 = NULL;
	SLList *L2 = NULL;
	SLInit(&L1);
	SLInit(&L2);
	for (i = 0; i < 3; i++)
	{
		SLAppend(L1, 0, a + i, sizeof(Term));
		SLAppend(L2, 0, b + i, sizeof(Term));
	}
	char buf[32] = { 0 };
	char choose = '1';
	while (choose != '0')
	{
		system("clear");
		menu();

		scanf("%s", buf);
		choose = buf[0];
		if (choose == '0')
		{
			break;
		}
		switch (choose)
		{
		case '1':
			SLWalk(L1, PrintItem, NULL);
			break;
		case '5':
			AddTerm(L1);
			break;
		case '2':
			SLWalk(L2, PrintItem, NULL);
			break;
		case '6':
			AddTerm(L2);
			break;
		case '3':
			Combine(L1, L2, 1);
			break;
		case '4':
			Combine(L1, L2, 2);
			break;
		deafult:
			printf("请输入正确的选项\n");
			break;
		}
		pause();
	}
}