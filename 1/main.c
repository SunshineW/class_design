#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<assert.h>

#include"myhead.h"

int main()
{
	SLList *L = NULL;
	SLInit(&L);
	readData(L);
	char buf[256] = {0};
	char choose = '1';
	while (choose != '0')
	{
		system("clear");
		menu();
		printf("Please choose : ");
		
		scanf("%s", buf);
		choose = buf[0];
		if (choose == '0')
		{
			SaveDataToFile(L);
			break;
		}
		switch (choose)
		{
			case '1':
				ShowStaff(L);
				break;
			case '2':
				AddStaff(L);
				break;
			case '3':
				DelStaff(L);
				break;
			case '4':
				UpdateStaff(L);
				break;
			case '5':
				Search(L);
				break;
			default:
				break;
		}
		pause();
	}
	SLDestory(L);
	return 0;
}