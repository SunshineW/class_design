#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#include "sort.h"

void pause()
{
	printf("Press any key to continue...");
	setbuf(stdin, NULL);
	getchar();
}
void menu()
{
	printf("\n");
	printf("           1.对商品名称进行排序\n");
	printf("           2.对单价进行排序\n");
	printf("           3.对销售量进行排序\n");
	printf("           4.对销售额进行排序\n");
	printf("           5.添加药品\n");
	printf("           6.清空销售信息\n");
	printf("           7.购买\n");
	printf("           0.退出\n");
	printf("\n");
	printf("请选择: ");
}

int main(int argc, char **argv)
{
	SequenList *L = Init();
	Read(L);
	char buf[256] = {0};
	char choose = '1';
	while (choose != '0')
	{
		system("clear");
		menu();	
		scanf("%s", buf);
		choose = buf[0];
		if (choose == '0') break;
		
		switch (choose)
		{
			case '1': NumSort(L); break;
			case '2': PriceSort(L); break;
			case '3': CountSort(L); break;
			case '4': SaleSort(L); break;
			case '5': AddMedicine(L); break;
			case '6': ClearSale(L); break;
			case '7': SellSystem(L); break;
			default : break;
		}
		pause();
	}
	Write(L);
	free(L);
}