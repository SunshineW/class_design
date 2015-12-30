#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<assert.h>

#include"myhead.h"

#define FILE_NAME "data.txt"

int PrintStaff(int tag, void *memory, void *args)
{
	Staff *s = memory;
	assert(tag == 0);
	printf("%-10s %-10s %-15s %-15s %-15s\n", s->id, s->name, s->tel, s->call, s->mail);
	return 0;
}

/* save a worker data to file*/
int SaveStaff(int tag, void *memory, void *args)
{
	FILE *file = args;
	fwrite(memory, sizeof(Staff), 1, file);
	return 0;
}

int FindStaff(int tag, void *memory, void *args)
{
	Staff *thisStaff = memory;
	Staff *s = args;
	assert(tag == 0);
	if (strcmp(s->id, thisStaff->id) == 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}	
}

/* search a worker by name, then print*/
int FindByName(int tag, void *memory, void *args)
{
	Staff *ss = memory;
	Staff *find = args;
	if (strcmp(find->name, ss->name) == 0)
	{
		printf("%-10s %-10s %-15s %-15s %-15s\n", ss->id, ss->name, ss->tel, ss->call, ss->mail);
	}
	return 0;
}

int readData(SLList *L)
{

	/* open data file ,or creat it if not exist */
	FILE *f = fopen(FILE_NAME, "rb+");
	if (f == NULL)
	{
		f = fopen(FILE_NAME, "wb+");
		if(f == NULL)
		{
			printf("creat data file failed!\n");
			exit(1);
		}
	}
	fread(&staffID, sizeof(staffID), 1, f);
	Staff temp;
	while(1)
	{
		fread(&temp, sizeof(temp), 1, f);
		if (feof(f))
		{
			break;
		}
		SLAppend(L, 0, &temp, sizeof(temp));
	}
	fclose(f);
	
	
}

void pause()
{
	printf("Press any key to continue...");
	setbuf(stdin, NULL);
	getchar();
}

/* printf all worker data*/
void ShowStaff(SLList *L)
{
	printf("%-10s %-10s %-15s %-15s %-15s\n","id","name", "phone", "call", "mail");
	SLWalk(L, PrintStaff, NULL);
}

void SaveDataToFile(SLList *L)
{
	FILE *fp = fopen(FILE_NAME, "wb");
	if (fp == NULL)
	{
		printf("open data file failed!\n");
	}
	else
	{
		fwrite(&staffID, sizeof(staffID), 1, fp);
		SLWalk(L,SaveStaff,fp);
	}
	fclose(fp);
	
}
void AddStaff(SLList *L)
{
	int result = SL_SUCCESS;
	Staff newStaff = { 0 };
	printf("Please input the name: ");
	scanf("%s", newStaff.name);
	printf("Please input the tell: ");
	scanf("%s", newStaff.tel);
	printf("Please input the call: ");
	scanf("%s", newStaff.call);	
	printf("Please input the mail: ");
	scanf("%s", newStaff.mail);
	
	sprintf(newStaff.id, "%d", ++staffID);
	result = SLAppend(L, 0, &newStaff, sizeof(Staff));
	if (result == SL_SUCCESS)
	{
		printf("add success!\n");
	}
	else
	{
		printf("add failure!\n");
	}
}

void DelStaff(SLList *L)
{
	SLList *removed = NULL;
	Staff p = { 0 };
	printf("please input the num: ");
	scanf("%s", p.id);
	removed = SLWalk(L, FindStaff, &p);
	if (removed == NULL)
	{
		printf("Do not exits!\n");
	}
	else
	{
		SLDeleteNext(SLGetPri(L,removed));
		printf("delete success!\n");
	}
}

void UpdateStaff(SLList *L)
{
	int result = SL_SUCCESS;
	SLList *oldStaff = NULL;
	Staff newStaff = { 0 };
	printf("please input the num: ");
	scanf("%s", newStaff.id);
	oldStaff = SLWalk(L, FindStaff, &newStaff);
	if (oldStaff == NULL)
	{
		printf("Do not exits!\n");
	}
	else
	{
		
		Staff newStaff;
		memcpy(&newStaff, oldStaff->object, sizeof(newStaff));
		
		system("clear");
		printf("%-10s %-15s %-15s %-15s\n", "name", "phone", "call", "mail");
		printf("%-10s %-15s %-15s %-15s\n", newStaff.name, newStaff.tel, newStaff.call, newStaff.mail);
		printf("which you want to change?\n");
		printf("1.name  2.phone  3.call  4.mail\n");
		printf("please choose: ");
		
		char choose = 0;
		scanf("%d", &choose);
		switch(choose)
		{
			case 1:
				printf("please input new name: ");
				scanf("%s", newStaff.name);
				break;
				
			case 2:
				printf("please input new phone: ");
				scanf("%s", newStaff.tel);
				break;
			case 3:
				printf("please input new call: ");
				scanf("%s", newStaff.call);
				break;
			case 4:
				printf("please input new mail: ");
				scanf("%s", newStaff.mail);
				break;
			default:
				printf("nothing has changed\n");
				break;
		}
		
		result = SLUpdate(oldStaff, 0, &newStaff, sizeof(Staff));
		if (result == SL_SUCCESS)
		{
			printf("update success!\n");
		}
	}
}


void Search(SLList *L)
{
	Staff s = {0};
	printf("please input name: ");
	scanf("%s", s.name);
	printf("%-10s %-10s %-15s %-15s %-15s\n","id","name", "phone", "call", "mail");
	SLWalk(L, FindByName, &s);
}
void menu()
{
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("           1.display all worker         \n");
	printf("           2.add a new worker          \n");
	printf("           3.delete a wroker     \n");
	printf("           4.change a worker data          \n");
	printf("           5.search a worker         \n");
	printf("           0.exit           \n");
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

}