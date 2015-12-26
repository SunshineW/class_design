#ifndef HUFFMAN_H_
#define HUFFMAN_H_

typedef struct
{
	char data;
	int wighted;
}letter_t;

typedef struct HTnode_t
{
	char code[32];
	char data;
	int wighted;
	int parent, left, right;
}HTnode_t;

int SAVE(HTnode_t *tree, int size, char *text_file, char *code_file);
/* 统计文本中字符出现的次数并存储到letter中 
  返回的是出现的字符种类数 */
int wighted_count(letter_t *letter, char *filename);

/* 从key文件中读取权值 */
int read_wighted(letter_t *letter, char *filename);

/* 保存权值到文件  */
int save_wighted(letter_t *letter, char *filename, int size);

int creat_tree(HTnode_t *tree, letter_t *letter, int size);

int print_tree(HTnode_t *tree, int size);

#endif