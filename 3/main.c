#include<stdio.h>
#include<stdlib.h>
#include "huffman.h"

static void menu()
{
	printf("       1.发送消息\n");
	printf("       2.接收消息\n");
	printf("       0.退出\n");
	printf("\n请选择：");
}

static void pause()
{
	printf("Press any key to continue...");
	setbuf(stdin, NULL);
	getchar();
}

int main(int argc, char **argv)
{
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
				send();
				break;
			case '2':
				receive();
				break;
			default:
				printf("输入错误\n");
		}
		pause();
	}
	return 0;
}