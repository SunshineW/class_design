#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "school.h"

void menu(void)
{
	printf("      1.列出景点\n");
	printf("      2.修改景点信息\n");
	printf("      3.查询路径\n");
	printf("      4.查询景点信息\n");
	printf("      0.退出\n");
	printf("\n");
	printf("请选择: ");
}
void pause(void)
{
	printf("Press any key to continue...");
	setbuf(stdin, NULL);
	getchar();
}
int main(int argc, char **argv)
{
	struct Graph *G;
	
	G = MakeGraph(List);
	Scenery_init(G);
	
	char buf[256] = {0};
	char choose = '1';
	while(1)
	{
		system("clear");
		menu();
		scanf("%s", buf);
		choose = buf[0];
		if (choose == '0')
		{
			break;
		}
		switch(choose)
		{
			case '1':
				ShowAllScen(G);
				break;
			case '2':
				UpdataScenery(G);
				break;
			case '3':
				MinPath(G);
				break;
			case '4':
			SearchScenery(G);
				break;
			default:
				printf("输入错误\n");
		}
		pause();
	}
	
	FreeScenery(G);
	FreeGraph(G);
	return 0;
}