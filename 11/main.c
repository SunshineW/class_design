#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "bin.h"

#define MAX_SIZE 20

struct print_node
{
	int data;
	int level;
};

int mi(int base, int count)
{
	if (count == 0)
	{
		return 1;
	}
	else
	{
		while(count > 1)
		{
			base *= 2;
			count--;
		}
	}
	return base;
	
	
}
void write(struct bin_node *node, struct print_node * pn, int index, int level)
{
	if(node == NULL)
	{
		return;
	}
	pn[index].data = node->data;
	pn[index].level = level;
	
	write(node->right, pn, index - mi(2, level-2), level - 1);
	write(node->left, pn, index + mi(2, level -2), level - 1);
}

int deepth(struct bin_node *node)
{
	if (node == NULL)
	{
		return 0;
	}
	return 1 + (deepth(node->left) >= deepth(node->right) \
				? deepth(node->left) \
				: deepth(node->right));			
}

void print_tree(struct bin_node *node)
{
	
	int i;
	int tree_deep = deepth(node);
	int tree_size = mi(2, tree_deep) - 1;
	struct print_node *pn = malloc(sizeof(struct print_node) * tree_size);
	if (pn == NULL)
	{
		printf("内存不足\n");
		return;
	}
	for(i=0; i<tree_size; i++)
	{
		pn[i].data = -1;
		pn[i].level = -1;
	}
	write(node, pn, mi(2 ,tree_deep - 1) - 1, tree_deep);
	for(i=0; i<tree_size; i++)
	{
		if (pn[i].level == -1)
			printf("\n");
		else
		{
			printf("%*s%-5d\n",(tree_deep - pn[i].level) * 10, "", pn[i].data);	
		}
	}
	printf("\n");
	free(pn);
}

void pause()
{
	printf("Press any key to continue...");
	setbuf(stdin, NULL);
	getchar();
}

void insert(struct bin_tree *tree)
{
	int a, result;
	printf("请输入您要插入的数据: ");
	scanf("%d", &a);
	result = bin_insert(tree, a);
	switch(result)
	{
		case 1: puts("插入成功"); break;
		case 2: puts("该数已存在"); break;
		case 0: puts("内存不足"); break;
	}
}

void delete(struct bin_tree *tree)
{
	int a, result;
	printf("请输入您要删除的数据: ");
	scanf("%d", &a);
	result = bin_delete(tree, a);
	switch(result)
	{
		case 1: puts("删除成功"); break;
		case 0: puts("该数未找到"); break;
	}
}

void search(struct bin_tree *tree)
{
	int a, result;
	printf("请输入您要查找的数据: ");
	scanf("%d", &a);
	result = bin_search(tree, a);
	switch(result)
	{
		case 1: puts("该数在此树中"); break;
		case 0: puts("该数不在此树中"); break;
	}
}

void traverse(struct bin_tree *tree)
{
	bin_walk(tree);
	puts("");
}

void count(struct bin_tree *tree)
{
	printf("树的节点数是: %d\n", bin_count(tree));
}

void menu()
{
	printf("\n");
	printf("        1.打印二叉树\n");
	printf("        2.插入操作\n");
	printf("        3.删除操作\n");
	printf("        4.查找\n");
	printf("        5.统计数的节点数\n");
	printf("        6.中序遍历\n");	
	printf("        0.退出\n");
	printf("\n");
	printf("请选择: ");
}

int main(int argc, char **argv)
{

	if (argc == 2 && strcmp(argv[1], "menu") == 0)
	{
		struct bin_tree *tree = bin_create();
		if (tree == NULL)
		{
			printf("内存不足\n");
			return 1;
		}
		
		char buf[32] = {0};
		char choose = '1';
		bin_insert(tree, 4);
		bin_insert(tree, 2);
		bin_insert(tree, 1);
		bin_insert(tree, 3);
		bin_insert(tree, 6);
		bin_insert(tree, 5);
		bin_insert(tree, 7);

		while (choose != '0')
		{
			system("clear");
			menu();	
			scanf("%s", buf);
			choose = buf[0];
			if (choose == '0') break;
			
			switch (choose)
			{
				case '1': print_tree(tree->root); break;
				case '2': insert(tree); break;
				case '3': delete(tree); break;
				case '4': search(tree); break;
				case '5': count(tree); break;
				case '6': traverse(tree); break;
				default : break;
			}
			pause();
		}
		bin_destroy(tree);
		return 0;		
	}
	
	else
	{
		int i, result;
		int a;
		int num[MAX_SIZE];
		int index = 0;
		char buf[128];
		memset(buf, 0, 256);
		
		struct bin_tree *tree = bin_create();
		if (tree == NULL)
		{
			printf("内存不足\n");
			return 1;
		}
		
		printf("请在下面输入一组数字(小于20个, 中间用空格隔开)\n");
		fgets(buf, 255, stdin);
		
		char *find =NULL;
		
		find = strtok(buf, " ");
		while(find != NULL)
		{
			num[index++] = atoi(find);
			find = strtok(NULL, " ");
		}
		for(i=0; i<index; i++)
		{
			bin_insert(tree, num[i]);
		}
		printf("平衡二叉树已建立完毕,下面是二叉树的结构\n\n");
		print_tree(tree->root);
		printf("中序遍历: ");
		bin_traverse(tree);
		printf("\n");
		printf("请输入一个数: ");
		scanf("%d", &a);
		result = bin_search(tree, a);
		if (result == 0)
		{
			printf("该数不在此二叉树中,现在已将其插入二叉树\n");
			bin_insert(tree, a);
			printf("中序遍历: ");
			bin_traverse(tree);
			printf("\n");

		}
		else if(result == 1)
		{
			printf("该数在此树中,现在已将其从树中删除\n");
			bin_delete(tree, a);
			printf("中序遍历: ");
			bin_traverse(tree);
			printf("\n");
		}
		bin_destroy(tree);
		return 0;	
	}
	
}