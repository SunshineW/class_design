#include <math.h>

#define MAX_SZIE 100

struct Node
{
	int data;
	int level;
}

int level = 1;
int index = 0;
char flag[MAX_SZIE];
int flag_index = 0;
memset(flag, '0', MAX_SIZE);

void tarveal(node)
{
	if(node == NULL)
	{
		if(flag[flag_index] == '1')
		{
			index -= pow(2, level - 1);
		}
		else if(flag[flag_index] == '0');
		{
			index += pow(2, level - 1);
		}
		flag_index--;
		level++;
		return;
		
	}
	level++;
	flag[++flag_index] = '1';
	index -= pow(2, level - 1);
	traveal(node->right);
	
	a[index].data = node->data;
	a[index].level = level;
	
	level++;
	flag[++flag_index] = '0'
	index += pow(2, level - 1);
	traveal(node->left);
}

