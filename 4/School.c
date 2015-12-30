#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "graphs.h"
#include "shpath.h"

#define GRAPH_NOTCONNECTED			INT_MAX

static int scenery_id = 100;
struct Scenery
{
	int id;
	char name[32];
	char summary[256];
};

int AddScenery(struct Graph *G)
{
	struct Scenery s = {0};
		
	printf("请输入景点的名称：");
	scanf("%s", s.name);
	printf("请写入简介: ");
	scanf("%s", s.summary);
	s.id = scenery_id;
	scenery_id++;
	
	int index =AddVertex(G);
	struct Scenery *ptr = malloc(sizeof(struct Scenery));
	if(!ptr) return 1;
	memcpy(ptr, &s, sizeof(struct Scenery));
	G->Vertices[index]->Tag.Ptr = ptr;
	return 0;
}

int ShowAllScen(struct Graph *G)
{
	int i;
	printf("------------------------------------------\n");
	printf("%-16s%-20s%s\n", "景点代号", "景点名称", "景点介绍");
	printf("------------------------------------------\n");
	for(i=0; i<G->NumVertices; i++)
	{
		struct Scenery *s = G->Vertices[i]->Tag.Ptr;
		printf("%-12d%-20s%s\n",s->id, s->name, s->summary);
	}
	printf("------------------------------------------\n");
}
int SearchScenery(struct Graph *G)
{
	system("clear");
	struct Scenery *s;
	int choose;
	int i;
	for(i=0; i<G->NumVertices; i++)
	{
		s = G->Vertices[i]->Tag.Ptr;
		printf("                 %d. %s\n", i + 1, s->name);
	}
	printf("请选择您要查看的景点：");
	scanf("%d", &choose);
	choose--;
	while(choose < 0 || choose >= G->NumVertices)
	{
		printf("请重新选择：");
		scanf("%d", &choose);
		choose--;
	}
	
	s = G->Vertices[choose]->Tag.Ptr;
	printf("景点代号：%d\n", s->id);
	printf("景点名称：%s\n", s->name);
	printf("景点介绍：%s\n", s->summary);
	return 0;
}
int AddPath(struct Graph *G)
{
	int i;
	int source, dest, cost;
	for(i=0; i<G->NumVertices; i++)
	{
		struct Scenery *s = G->Vertices[i]->Tag.Ptr;
		printf("%d. %s\n",i + 1, s->name);
	}
	printf("请选择道路的起点：");
	scanf("%d", &source);
	source--;
	printf("请选择道路的终点：");
	scanf("%d", &dest);
	dest--;
	printf("请输入路径的长度：");
	scanf("%d", &cost);
	
	ConnectVertex(G, source, dest, cost);
	ConnectVertex(G, dest, source, cost);
	return 0;
}

int UpdataScenery(struct Graph *G)
{
	int i;
	int not_find = 1;
	printf("请输入要修改的景点的代号: ");
	int update_id;
	scanf("%d", &update_id);
	for(i=0; i<G->NumVertices; i++)
	{
		struct Scenery *s = G->Vertices[i]->Tag.Ptr;
		if(s->id == update_id)
		{
			system("clear");
			printf("原信息如下\n");
			printf("名称：%s\n", s->name);
			printf("简介：%s\n", s->summary);
			
			char buf[256];
			printf("请输入新的信息(不修改某项请输入0)\n");
			printf("名称：");
			scanf("%s", buf);
			if (buf[0] != '0')
			{
				memcpy(s->name, buf, strlen(buf) + 1);
			}
			printf("简介：");
			scanf("%s", buf);
			if (buf[0] != '0')
			{
				memcpy(s->summary, buf, strlen(buf) + 1);
			}
			printf("信息更新完成\n");
			printf("名称：%s\n", s->name);
			printf("简介：%s\n", s->summary);
			
			not_find = 0;
			break;
		}
	}
	if (not_find)
	{
		printf("该代号不存在!\n");
	}
}

int MinPath(struct  Graph *G)
{
	system("clear");
	int i;
	int source, dest, cost;
	struct Dijkstra_Table T;
	struct Scenery *s;
	
	for(i=0; i<G->NumVertices; i++)
	{
		s = G->Vertices[i]->Tag.Ptr;
		printf("                 %d. %s\n", i + 1, s->name);
	}
	
	printf("请选择起点：");
	scanf("%d", &source);
	source--;
	while(source < 0 || source >= G->NumVertices)
	{
		printf("请重新选择起点：");
		scanf("%d", &source);
		source--;
	}
	printf("请选择终点：");
	scanf("%d", &dest);
	dest--;
	while(dest < 0 || dest >= G->NumVertices)
	{
		printf("请重新选择终点：");
		scanf("%d", &dest);
		dest--;
	}
	
	Dijkstra_InitTable(&T);
	Dijkstra_Simple(G, dest, &T);
	printf("最短路径是：");
	int v = source;
	while(T.Results[v].Previous != -1)
	{
		s = G->Vertices[v]->Tag.Ptr;
		printf("%s-->", s->name);
		v = T.Results[v].Previous;
	}
	s = G->Vertices[v]->Tag.Ptr;
	printf("%s\n", s->name);
	printf("总长度：%d\n", T.Results[source].Total);
	Dijkstra_FreeTable(&T);
}

int FreeScenery(struct Graph *G)
{
	int i;
	struct EdgeScan E;
	for(i=0; i<G->NumVertices; i++)
	{
		EdgeScanStart(G, 0, &E);
		while(!EdgeScanNext(&E))
		{
			struct Scenery *freenode = G->Vertices[E.Dest]->Tag.Ptr;
			free(freenode);
			G->Vertices[E.Dest]->Tag.Ptr = NULL;
		}
		EdgeScanEnd(&E);
	}
}

int Scenery_init(struct Graph *G)
{
	int i;
	int index;
	struct Scenery *ptr;
	
	struct Scenery s[10]=
	{
		{101, "国教楼", "无"},
		{102, "北大坑", "无"},
		{103, "北苑餐厅", "无"},
		{104, "体育馆", "无"},
		{105, "中心体育场", "无"},
		{106, "中升路", "无"},
		{107, "弘德广场", "无"},
		{108, "图书馆", "无"},
		{109, "紫薇广场", "无"},
		{110, "滴水涌泉", "无"},
	};
	scenery_id = 110;
	for(i=0; i<10; i++)
	{
		index = AddVertex(G);
		if (index < 0) exit(EXIT_FAILURE);
		
		ptr = malloc(sizeof(struct Scenery));
		if(!ptr) exit(EXIT_FAILURE);
		
		memcpy(ptr, s + i, sizeof(struct Scenery));
		G->Vertices[index]->Tag.Ptr = ptr;
	}
	ConnectVertex(G,1-1,2-1,3);
	ConnectVertex(G,1-1,3-1,5);
	ConnectVertex(G,2-1,1-1,3);
	ConnectVertex(G,2-1,3-1,5);
	ConnectVertex(G,3-1,1-1,5);
	ConnectVertex(G,3-1,4-1,6);
	ConnectVertex(G,3-1,5-1,7);
	ConnectVertex(G,4-1,3-1,6);
	ConnectVertex(G,4-1,5-1,2);
	ConnectVertex(G,5-1,3-1,7);
	ConnectVertex(G,5-1,4-1,2);
	ConnectVertex(G,5-1,6-1,5);
	ConnectVertex(G,5-1,7-1,5);
	ConnectVertex(G,5-1,8-1,6);
	ConnectVertex(G,6-1,5-1,5);
	ConnectVertex(G,6-1,9-1,6);
	ConnectVertex(G,7-1,5-1,5);
	ConnectVertex(G,7-1,8-1,2);
	ConnectVertex(G,8-1,5-1,6);
	ConnectVertex(G,8-1,7-1,2);
	ConnectVertex(G,8-1,9-1,3);
	ConnectVertex(G,9-1,6-1,6);
	ConnectVertex(G,9-1,8-1,3);
	ConnectVertex(G,9-1,10-1,3);
	ConnectVertex(G,10-1,9-1,3);
	return 0;	
}