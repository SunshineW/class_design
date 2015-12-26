#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

#include "huffman.h"
#define TXT_FILE 	"massage.txt"
#define KEY_FILE 	"massage.key"
#define CODE_FILE 	"message.code"

void check_null(void *p, char *message)
{
	if (p == NULL)
	{
		printf(message);
		printf("\n");
		exit(EXIT_FAILURE);
	}
}

/* 统计文本中字符出现的次数并存储到letter中 
  返回的是出现的字符种类数 */
int wighted_count(letter_t *letter, char *filename)
{
	int count[256] = {0};
	letter_t temp[256] = {0};
	int size = 0;
	int i, j;
	
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("open %s failed\n", filename);
		exit(1);
	}
	while(1)
	{
		int a = fgetc(fp);
		if (feof(fp) != 0)
		{
			break;
		}
		if (a>256 || a < 0)
		{
			printf("只支持压缩英文消息\n");
			exit(EXIT_FAILURE);
		}
		count[a]++;
	}
	fclose(fp);
	
	for(i=0; i<256; i++)
	{
		if (count[i] != 0)
		{
			temp[size].data = i;
			temp[size].wighted = count[i];
			size++;
		}
	}
	
	memset(letter, 0, sizeof(letter_t) * size);
	for (i=0; i<size; i++)
	{
		for(j=i; j<size; j++)
		{
			if (temp[i].wighted >temp[j].wighted)
			{
				letter_t exchange = temp[i];
				temp[i] = temp[j];
				temp[j] = exchange;
			}
		}
		letter[i] = temp[i];
	}
	return size;
}

/* 从key文件中读取权值 */
int read_wighted(letter_t *letter, char *filename)
{
	FILE *fp;
	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("open %s failed\n", filename);
		exit(1);
	}
	int count=0;
	while(1)
	{
		
		fread(&(letter[count]), sizeof(letter_t), 1, fp);
		if(feof(fp))
		{
			break;
		}
		count++;
	}
	fclose(fp);
	return count;
}

/* 保存权值到文件  */
int save_wighted(letter_t *letter, char *filename, int size)
{
	int ret;
	FILE *fp;
	fp = fopen(filename, "wb");
	if (fp == NULL)
	{
		printf("creat key file failed\n");
		exit(1);
	}
	ret = fwrite(letter, sizeof(letter_t), size, fp);
	if (ret != size)
	{
		printf("write wrighted failed\n");
		exit(1);
	}
	fclose(fp);
	return 0;
}

int creat_tree(HTnode_t *tree, letter_t *letter, int size)
{
	int i, j, tree_size;
	tree_size = size * 2 - 1;
	
	for (i=0; i< size; i++)
	{
		tree[i].data = letter[i].data;
		tree[i].wighted = letter[i].wighted;
		tree[i].left = tree[i].right = -1;
		tree[i].parent = 0;
	}
	for(i=size; i<tree_size; i++)
	{
		tree[i].data = '*';
		tree[i].wighted = 0;
		tree[i].left = tree[i].right = -1;
		tree[i].parent = 0;
	}
	
	/* 构建好哈弗曼树结构 */
	for (i=size; i< tree_size; i++)
	{
		int min1, min2; /* 权值最小的两个 min1<min2 */
		                
		/*初始化 min1, min2 */
		for (j=0; j<i; j++)
		{
			if (tree[j].parent == 0)
			{
				break;
			}
		}
		min1 = j;
		
		for(j=j+1; j<i; j++)
		{
			if (tree[j].parent == 0)
			{
				break;
			}			
		}
		min2 = j; 
		
		for(j=j+1; j<i; j++) /* loop1: 求数组中最小的两个数*/
		{
			if(tree[j].parent == 0)
			{
				if (tree[j].wighted < tree[min2].wighted)
				{
					if (tree[j].wighted < tree[min1].wighted)
					{
						min2 = min1;
						min1 = j;
					}
					else
					{
						min2 = j;
					}
				}
			}
		} /* end of loop1 */

		tree[i].wighted = tree[min1].wighted + tree[min2].wighted;
		tree[i].left = min1;
		tree[i].right = min2;
		tree[min1].parent = i;
		tree[min2].parent = i;
	}
	
	/* 把哈弗曼编码写入节点中 */
	for(i=size*2 -2 ; i >= 0; i--)
	{
		char buf[32];
		memset(buf, '0', 32);
		int parent = tree[i].parent;
		int child = i;
		j = 31;
		while(parent != 0)
		{
			if (tree[parent].left == child)
			{
				buf[j]='1';
			}
			else
			{
				buf[j]='0';
			}
			child = parent;
			parent = tree[parent].parent;
			j--;
		}
		memcpy(tree[i].code, buf + j + 1, 31 - j);
		tree[i].code[31- j] = '\0';
	}

}

int print_tree(HTnode_t *tree, int size)
{
	int i;
	int tree_size = size * 2 -1;
	printf("%-8s%-6s%-16s%s\n", "wighted", "ascii", "code", "character");
	for(i=0; i<size; i++)
	{
		//if (tree[i].left != -1)
		{
			printf("%-8d%-6d%-16s%c\n", tree[i].wighted, tree[i].data, tree[i].code, tree[i].data);
		}
	}
	return 0;
}

int encrypt(HTnode_t *tree, int size, char *text_file, char *code_file)
{
	/* 先把要压缩的文本读到内存(source_buf)中,再编码
	   成01字符串写入到另一块内存(code_buf)中，最后每
	   八位一组字符串压缩成一个字符写入到(code)中 */
	int i, j;
	int code_buf_index;
	int code_index;
	char *source_buf;
	char *code_buf;
	unsigned char *code;
	long txt_length;
	
	FILE *r = fopen(text_file, "rb");
	check_null(r, "fopen");
	fseek(r, 0, SEEK_END);
	txt_length = ftell(r);
	fseek(r, 0, SEEK_SET);
	
	source_buf = malloc(txt_length);
	check_null(source_buf, "malloc");
	fread(source_buf, txt_length, 1, r);
	
	fclose(r);
	
	code_buf = malloc(txt_length * 8);
	check_null(code_buf, "malloc");
	memset(code_buf, 0, txt_length * 8);
	
	code_buf_index = 0;
	for(i=0; i<txt_length; i++)
	{
		for(j=size-1; j>=0; j--)
		{
			if(source_buf[i] == tree[j].data)
			{
				memcpy(code_buf + code_buf_index, tree[j].code, strlen(tree[j].code));
				code_buf_index += strlen(tree[j].code);
				break;
			}
		}
	}
	
	int code_length = code_buf_index / 8 + 1 + sizeof(long); 
	printf("要发送的文字为：\n%s\n", source_buf);
	
	code = malloc(code_length);
	check_null(code, "malloc");
	memset(code, 0, code_length);
	
	code_index = 0;
	memcpy(code, &txt_length, sizeof(long));
	code_index += sizeof(long);
	
	
	
	/* 每八个一组0,1字符压缩到一个字符中，
	   最后一组不足八位补零够八位后压缩 */
	for(i=0; i<code_buf_index / 8 + 1; i++)
	{
		unsigned char temp_storage;
		char buf[9];
		memcpy(buf, code_buf + 8 * i, 8);
		for(j=0;j<8;j++)
		{
			if (buf[j] == '0')
				temp_storage &= ~(1<<j);
			
			else if (buf[j] == '1')
				temp_storage |= (1<<j);
			
			else
				temp_storage &= ~(1<<j);
		}
		code[code_index] = temp_storage;
		code_index++;
	}
	FILE *w = fopen(code_file, "wb");
	check_null(w, "fopen,wb");
	fwrite(code, code_length, 1, w);
	fclose(w);
	printf("压缩发送成功\n");
	printf("压缩前长度:%d, ", txt_length);
	printf("压缩后长度:%d, ", code_length);
	printf("压缩比:%.2f\n", (code_length * 1.00) / txt_length);
		
	free(code_buf);
	free(code);
	free(source_buf);
	return 0;
}

int decipher(HTnode_t *tree, int size, char *codefile)
{
	int i, j;
	long txt_length;
	int code_length;
	int code_index;
	int code_buf_index;
	int code_buf_length;
	unsigned char *code = NULL;
	char *code_buf = NULL;
	char *txt = NULL;
	
	FILE *r = fopen(codefile, "rb");
	check_null(r, "open codefile");
	
	fseek(r, 0, SEEK_END);
	
	code_length = ftell(r); /* 压缩文本的长度 */
	
	fseek(r, 0, SEEK_SET);
	code = malloc(code_length);
	check_null(code, "malloc");
	fread(code, code_length, 1, r);
	fclose(r);
	
	code_index = 0;
	memcpy(&txt_length, code, sizeof(long));
	code_index += sizeof(long);
	
	txt = malloc(txt_length + 1);
	check_null(txt, "malloc");
	memset(txt, 0, txt_length);
	
	code_buf = malloc((code_length - sizeof(long)) * 8);
	check_null(code_buf, "malloc");
	
	code_buf_index = 0;
	while(code_index < code_length)
	{
		unsigned char temp_storage;
		temp_storage = code[code_index];
		
		for(j=0; j<8; j++)
		{
			if(temp_storage == (temp_storage | (1<<j)))
			{
				code_buf[code_buf_index] = '1';	
			}
			else if(temp_storage == (temp_storage & ~(1<<j)))
			{
				code_buf[code_buf_index] = '0';
			}
			else
			{
				printf("?");
				printf("文件格式错误\n");
				exit(EXIT_FAILURE);
			}
			code_buf_index++;
		}
		code_index++;
	}
	int leave = txt_length;
	code_buf_length = code_buf_index;
	code_buf_index = 0;
	while(code_buf_index < code_buf_length && leave >0)
	{
		int find; 				/* 哈弗曼树的节点下标 */
		find = size * 2 - 2; 	/* 哈弗曼树根节点下标 */
		while(1)
		{
			if(find == -1) /* 没找到编码对应的字符，说明已解码到最后一组八位字符串*/
			{
				goto end;
			}
			if(tree[find].left == -1)  /* 找到对应的字符*/
			{
				txt[txt_length - leave] = tree[find].data;
			//	printf("%c", tree[find].data);
				leave--;
				break;
			}
			else
			{
				if(code_buf[code_buf_index] == '1')
				{
					find = tree[find].left;
				}
				else if(code_buf[code_buf_index] == '0')
				{
					find = tree[find].right;
				}
				else
				{
				
				}
				code_buf_index++;
			}
		}
	}
	txt[txt_length] = '\0';
	
end:
	printf("接收到的数据为:\n");
	printf("%s\n", txt);
	free(code);
	free(code_buf);
	free(txt);
	return 0;
	
}

void send()
{
	int buf_size = 2048;
	char buf[buf_size];
	
	FILE *fp = fopen(TXT_FILE, "w");
	if (fp == NULL)
	{
		printf("init faild\n");
		exit(EXIT_FAILURE);
	}
	printf("Please input text: (type \"exit\" to exit)\n");
	while(1)
	{
		fgets(buf, buf_size, stdin);
		if (strcmp(buf, "exit\n") == 0)
		{
			break;
		}
		fwrite(buf, strlen(buf), 1, fp);
	}
	fclose(fp);
	
	
	letter_t letter[128];
	int size = wighted_count(letter, TXT_FILE);

	HTnode_t tree[size * 2 - 1];
	creat_tree(tree, letter, size);
	
	save_wighted(letter, KEY_FILE, size);
	encrypt(tree, size, TXT_FILE, CODE_FILE);
}

void receive()
{
	letter_t letter[128];
	int size = read_wighted(letter, KEY_FILE);
	HTnode_t tree[size * 2 - 1];

	creat_tree(tree, letter, size);
	decipher(tree, size, CODE_FILE);
}