#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "sort.h"

#define MAX_SZIE 100
#define DATA_FILE "medicine.data"


static void Swap(Node *a, Node *b)
{
    Node t = *a; *a = *b; *b = t;
}
  
static void PercDown(Node A[], int p, int N)
{
  /* 将N个元素的数组中以A[p]为根的子堆调整为最大堆 */
    int Parent, Child;
    Node X;
 
    X = A[p]; /* 取出根结点存放的值 */
    for( Parent=p; (Parent*2+1)<N; Parent=Child ) {
        Child = Parent * 2 + 1;
        if( (Child!=N-1) && (A[Child].sale<A[Child+1].sale) )
            Child++;  /* Child指向左右子结点的较大者 */
        if( X.sale >= A[Child].sale ) break; /* 找到了合适位置 */
        else  /* 下滤X */
            A[Parent] = A[Child];
    }
    A[Parent] = X;
}
 
static void HeapSort(Node A[], int N ) 
{ 
     int i;
       
     for ( i=N/2-1; i>=0; i-- )/* 建立最大堆 */
         PercDown( A, i, N );
      
     for ( i=N-1; i>0; i-- ) {
         /* 删除最大堆顶 */
         Swap( &A[0], &A[i] ); /* 见代码7.1 */
         PercDown( A, 0, i );
     }
}

/* 对销售额进行排序, 堆排序 */
void SaleSort(SequenList *L)
{
	int i, j;
	int size = L->length;
	Node *sort = malloc(sizeof(Node) * size);
	if (sort == NULL)
	{
		printf("内存不足\n");
		exit(1);
	}
	memcpy(sort, L->r, sizeof(Node) * size);
	
	HeapSort(sort, size);
	ShowAllMedicine(sort, size);
	free(sort);
}

/* 对单价进行排序,冒泡排序 */
void PriceSort(SequenList *L)
{
	int i, j;
	int size = L->length;
	Node *sort = malloc(sizeof(Node) * size);
	if (sort == NULL)
	{
		printf("内存不足\n");
		exit(1);
	}
	memcpy(sort, L->r, sizeof(Node) * size);

	Node temp;
	for(i=0; i<size; i++)
	{
		for(j=i; j<size; j++)
		{
			if (sort[i].price > sort[j].price)
			{
				temp = sort[i];
				sort[i] = sort[j];
				sort[j] = temp;
			}				
		}
	}
	
	ShowAllMedicine(sort, size);
	free(sort);
}

static void QuickSort(Node * A, int l, int r)
{
    int loc;

    if (l < r) 
	{
        int i = l, j = r;
        Node tmp, pivot = A[l];
        for (;;) 
		{
            while (A[j].count >= pivot.count && j > l)
                j--;
            while (A[i].count < pivot.count && (i < r))
                i++;
            if (i < j) 
			{
                tmp = A[i];
                A[i] = A[j];
                A[j] = tmp;
            } 
			else 
			{
                loc = j;
                break;
            }
        }
        QuickSort(A, l, loc);
        QuickSort(A, loc + 1, r);
    }
}

/* 对销售量进行排序, 快速排序 */
void CountSort(SequenList *L)
{
	int i, j;
	int left, right, middle;
	int size = L->length;
	Node *sort = malloc(sizeof(Node) * size);
	if (sort == NULL)
	{
		printf("内存不足\n");
		exit(1);
	}
	memcpy(sort, L->r, sizeof(Node) * size);
	
	QuickSort(sort, 0, size - 1);
	ShowAllMedicine(sort, size);
		
}

/* 返回基数排序中的桶次序 */
static int GetDigit (char *num, int D)
{ 
	int index = 4 - D;
	if (num[index] >= 'A' && num[index] <= 'Z')
	{
		return num[index] - 'A';
	}
	else
	{
		return num[index] - '0' + 26;
	}
}

/* 对药品编号进行排序, 基数排序 */
void NumSort(SequenList *L)
{ 
	int size = L->length;
	Node *sort = malloc(sizeof(Node) * size);
	if (sort == NULL)
	{
		printf("内存不足\n");
		exit(1);
	}
	memcpy(sort, L->r, sizeof(Node) * size);
	
    int D, Di, i;
    Bucket B;
    PtrToBNode tmp, p, List = NULL; 
     
    for (i=0; i<36; i++) /* 初始化每个桶为空链表 */
        B[i].head = B[i].tail = NULL;
		
    for (i=0; i<size; i++) 
	{ /* 将原始序列逆序存入初始链表List */
        tmp = (PtrToBNode)malloc(sizeof(struct BNode));
        tmp->key = sort[i];
        tmp->next = List;
        List = tmp;
    }
    /* 下面开始排序 */ 
	
    for (D=1; D <= 4 ; D++) 
	{ /* 对数据的每一位循环处理 */
       /* 下面是分配的过程 */
        p = List;
        while (p)
		{
            Di = GetDigit(p->key.num, D); /* 获得当前元素的当前位数字 */
            /* 从List中摘除 */
            tmp = p; p = p->next;
            /* 插入B[Di]号桶尾 */
            tmp->next = NULL;
            if (B[Di].head == NULL)
                B[Di].head = B[Di].tail = tmp;
            else 
			{
                B[Di].tail->next = tmp;
                B[Di].tail = tmp;
            }
        }
        /* 下面是收集的过程 */
        List = NULL; 
        for (Di=36-1; Di>=0; Di--) 
		{ /* 将每个桶的元素顺序收集入List */
            if (B[Di].head) 
			{ /* 如果桶不为空 */
                /* 整桶插入List表头 */
                B[Di].tail->next = List;
                List = B[Di].head;
                B[Di].head = B[Di].tail = NULL; /* 清空桶 */
            }
        }
    }
    /* 将List倒入sort[]并释放空间 */
    for (i=0; i<size; i++) 
	{
       tmp = List;
       List = List->next;
       sort[i] = tmp->key;
       free(tmp);
    } 
	ShowAllMedicine(sort, size);
	free(sort);
}

/* 显示所有药品信息 */
void ShowAllMedicine(Node *arr, int size)
{
	int i;
	printf("%-12s%-12s%-12s%-10s%s\n"
		, "ID", "NAME", "PRICE", "COUNT", "SALES");
	printf("----------------------------------------------------------\n");
	for(i=0; i<size; i++)
	{
		printf("%-12s%-12s%-12.2f%-10d%-.2f\n"
				,arr[i].num
				,arr[i].name
				,arr[i].price
				,arr[i].count
				,arr[i].sale);
	}
	printf("----------------------------------------------------------\n");
}
 
/* 药品信息写入文件 */
int Write(SequenList *L)
{
	FILE *fp = fopen(DATA_FILE, "wb");
	if (fp == NULL)
	{
		printf("open datafile error\n");
		exit(1);
	}
	fwrite(L->r, sizeof(Node), L->length, fp);
	fclose(fp);
}

/* 读取药品信息 */
int Read(SequenList *L)
{
	FILE *fp = fopen(DATA_FILE, "rb");
	if (fp == NULL)
	{
		fp = fopen(DATA_FILE, "wb+");
		if (fp == NULL)
		{
			printf("creat datafile error\n");
			exit(1);
		}
	}
	while(1)
	{
		Node temp;
		fread(&temp, sizeof(Node), 1, fp);
		if (feof(fp)) break;
		
		L->r[L->length++] = temp;
	}
	fclose(fp);
}

/* 创建一个顺序表, 不用后记得释放内存*/
SequenList *Init(void)
{
	int i;
	SequenList * L;
	L = malloc(sizeof(SequenList));
	if (L == NULL)
	{
		printf("out of memory\n");
		return NULL;
	}
	L->length = 0;
	for(i=0; i< MAX_SZIE; i++)
	{
		strcpy(L->r[i].num, "");
		strcpy(L->r[i].name, "");
		L->r[i].price = 0.00;
		L->r[i].count = 0;
		L->r[i].sale = 0.00;
	}
	return L;
}

/* 清空销售信息 */
void ClearSale(SequenList *L)
{
	int i;
	for(i=0; i< L->length; i++)
	{
		L->r[i].count = 0;
		L->r[i].sale = 0.00;
	}
	printf("销售数据已清空\n");
}

/* 添加药品信息 */
void AddMedicine(SequenList *L)
{
	if (L->length >= MAX_SZIE)
	{
		printf("已达到最大,不能再添加新药品\n");
		return;
	}
	struct Node node = {0};
	printf("请输入药品的编号: ");
	scanf("%s",node.num);
	printf("请输入药品名称: ");
	scanf("%s", node.name);
	printf("请输入药品单价: ");
	scanf("%f", &node.price);
	L->r[L->length++] = node;
	printf("添加成功\n");
}

/* 模拟购物系统 */
void SellSystem(SequenList *L)
{
	int i;
	float sum = 0;
	int count[L->length];
	for(i=0; i<L->length; i++)
	{
		count[i] = 0;
	}
	for(i=0; i<L->length; i++)
	{
		printf("%d.%10s  price:%10.2f\n", i + 1, L->r[i].name, L->r[i].price);
	}
	while (1)
	{
		int index;
		int num;
		printf("请选择您要购买的商品(输入0结账): ");
		scanf("%d", &index);
		if(index == 0) break;
		while(index < 0 || index > L->length)
		{
			printf("请选择您要购买的商品(输入0结账): ");
			scanf("%d", &index);
		}

		printf("请输入数量: ");
		scanf("%d", &num);
		count[--index] += num;
	}
	system("clear");
	printf("您的购物清单\n");
	printf("--------------------------------------------------\n");
	printf("%12s%12s%12s%12s\n", "NAME", "PRICE", "COUNT", "TOTAL");
	printf("--------------------------------------------------\n");
	for(i=0; i<L->length; i++)
	{
		if(count[i] != 0)
		{
			float total = L->r[i].price * count[i];
			printf("%12s%12.2f%12d%12.2f\n"
					,L->r[i].name
					,L->r[i].price
					,count[i]
					,total);
					
			L->r[i].count += count[i];
			L->r[i].sale += total;
			sum += total;
		}
	}
	printf("--------------------------------------------------\n");
	printf("%36s%12.2f\n", "TOTAL:", sum);
	printf("--------------------------------------------------\n");
	
}

