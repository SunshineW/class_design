#ifndef SORT_H_
#define SORT_H_

#define MAX_SZIE 100

typedef struct Node
{
	char num[10];
	char name[10];
	float price;
	int count;
	float sale;
}Node;

typedef struct SequenList
{
	Node r[MAX_SZIE];
	int length;
}SequenList;

/* 基数排序 桶节点 */
typedef struct BNode *PtrToBNode;
struct BNode {
    Node key;
    PtrToBNode next;
};

struct HeadNode {
    PtrToBNode head, tail;
};

typedef struct HeadNode Bucket[36];

/* 对药品代号经行排序, 基数排序 */
void NumSort(SequenList *L);  
static int GetDigit (char *num, int D);

/*对销售额进行排序, 堆排序 */
void SaleSort(SequenList *L);
static void Swap(Node *a, Node *b) ;
static void PercDown(Node A[], int p, int N);
static void HeapSort(Node A[], int N);

/* 对单价进行排序, 冒泡排序 */
void PriceSort(SequenList *L);

/* 对销售数量进行排序, 快速排序 */
void CountSort(SequenList *L);
static void QuickSort(Node * A, int l, int r);


void ShowAllMedicine(Node *arr, int size);

void AddMedicine(SequenList *L);
void SellSystem(SequenList *L);
int Write(SequenList *L);
int Read(SequenList *L);
SequenList *Init(void);
void ClearSale(SequenList *L);

#endif