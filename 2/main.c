#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<time.h>

#include "stack.h"
#include "queue.h"

#define PARK_SUCCESS	0
#define STOP_FULL 		1
#define PAVEMENT_FULL 	2

#define STACK_MAX_SIZE 	5
#define QUENE_MAX_SIZE 	5

typedef struct CAR
{
	char plate_number[10];
	time_t start, end;
}CAR;

void ExitFailure(char *message)
{
	printf(message);
	exit(EXIT_FAILURE);
}
void CarIn(STACK *stack, QUEUE *queue)
{
	if (QueueCount(queue) < QUENE_MAX_SIZE)
	{
		int Result;
		CAR car = {0};
		printf("请输入车牌号： ");
		scanf("%s", car.plate_number);
		car.start = time(NULL);
		if (car.start == (time_t)-1)
		{
			printf("时间不可用\n");
			exit(EXIT_FAILURE);
		}
		
		if(StackCount(stack) < STACK_MAX_SIZE)
		{
			Result = StackPush(stack, 0, &car, sizeof(car));
			if (Result == STACK_SUCCESS)
			{
				printf("%s已经驶入停车场\n", car.plate_number);
			}
			else
			{
				printf("系统故障\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			Result = QueueAdd(queue, 0, &car, sizeof(car));
			if (Result == QUEUE_SUCCESS)
			{
				printf("停车场已满，车子停在便道等候\n");
			}
			else
			{
				printf("系统故障\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		printf("停车场和便道已满，无法停车\n");
	}
}

void ShowPark(STACK *stack, QUEUE *queue)
{	
	int i;
	int stack_count = StackCount(stack);
	int queue_count = QueueCount(queue);
	STACK temp_stack = {0};
	CAR car;
	if(stack_count > 0)
	{
		printf("停车场现在有%d车, ", stack_count);
		printf("从北往南依次是: ");
		for(i=0; i<stack_count; i++)
		{
			StackPop(&car, stack);
			StackPush(&temp_stack, 0, &car, sizeof(car));
		}
		for(i=0; i<stack_count; i++)
		{
			StackPop(&car, &temp_stack);
			printf("%s ", car.plate_number);
			StackPush(stack, 0, &car, sizeof(car));
		}

		printf("\n");
		
		if(queue_count > 0)
		{
			printf("便道处有%d辆车, ", queue_count);
			printf("从前面到后面依次是: ");
			for(i=0; i<queue_count; i++)
			{
				QueueRemove(&car, queue);
				printf("%s ", car.plate_number);
				QueueAdd(queue, 0, &car, sizeof(car));
			}
			printf("\n");
		}
	}
	else
	{
		printf("停车场没有车\n");
	}
}

void CarOut(STACK *stack, QUEUE *queue)
{	
	int i;
	int Result;
	size_t size  = sizeof(CAR);
	STACK temp_stack = {0};
	CAR car = {0};
	int stack_count = StackCount(stack);
	int queue_count = QueueCount(queue);

	char plate_number[10];
	printf("请输入车牌号：");
	scanf("%s", plate_number);
	
	/* 查找要离去的车 */
	Result = STACK_SUCCESS;
	int find = 0;
	for(i=0; i<stack_count; i++)
	{
		Result = StackPop(&car, stack);	
		if (strcmp(car.plate_number, plate_number) == 0)
		{
			find = 1;
			break;
		}
		Result = StackPush(&temp_stack, 0, &car, size); /* 外面的车压入临时栈 */
	}
	if(find)
	{
		/* 临时栈再依次pop到停车场里 */
		int temp_stack_count = StackCount(&temp_stack);
		for(i=0; i<temp_stack_count; i++)
		{
			Result = StackPop(&car, &temp_stack);
			Result = StackPush(stack, 0, &car, size);
		}
	
		car.end = time(NULL);
		if (car.end == (time_t)-1)
		{
			printf("时间不可用\n");
			exit(EXIT_FAILURE);
		}
		printf("%s停了%.2f 小时, ",plate_number, difftime(car.end, car.start) * 0.1); 
		printf("需要支付%.2f元\n", difftime(car.end, car.start) * 0.2);
		
		/*便道里的第一辆车驶入停车场*/
		if (queue_count > 0)
		{
			QueueRemove(&car, queue);
			StackPush(stack, 0, &car, size);
		}
	}
	else /* 在便道中寻找车 */
	{
		/* 临时栈再依次pop到停车场里 */
		int temp_stack_count = StackCount(&temp_stack);
		for(i=0; i<temp_stack_count; i++)
		{
			Result = StackPop(&car, &temp_stack);
			Result = StackPush(stack, 0, &car, size);
		}
		/* 队列中车辆全部出列再加到队尾，相当于遍历队列*/
		for(i=0; i<queue_count; i++)
		{
			QueueRemove(&car, queue);
			if (strcmp(car.plate_number, plate_number) == 0)
			{
				find = 1;
			}
			QueueAdd(queue, 0, &car, size);
		}	
		if (find)
		{
			printf("%s现在在便道中，不能驶出\n", car.plate_number);
		}
		else
		{
			printf("%s不在此停车场中\n", plate_number);
		}
	}
	StackDestroy(&temp_stack);
}

void menu()
{
	printf("       1.驶入\n");
	printf("       2.驶出\n");
	printf("       3.查看停车场使用情况\n");
	printf("       0.退出\n");
	printf("\n请选择：");
}
void pause()
{
	printf("Press any key to continue...");
	setbuf(stdin, NULL);
	getchar();
}
int main(int argc, char **argv)
{ 
	int i=1;
	size_t size;
	QUEUE queue = {0};
	STACK stack = {0};
	
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
				CarIn(&stack, &queue);
				break;
			case '2':
				CarOut(&stack, &queue);
				break;
			case '3':
				ShowPark(&stack, &queue);
				break;
			default:
				printf("输入错误\n");
		}
		pause();
	}
	StackDestroy(&stack);
	QueueDestroy(&queue);
	return 0;
}
