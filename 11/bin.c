#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include<assert.h>

#include"bin.h"
/* Creates and returns a new binary tree.
 * Returns a null  pointer ig a memory 
 * allocation error occurs. 
 */
struct bin_tree *bin_create(void)
{
	struct bin_tree *tree = malloc(sizeof* tree);
	if (tree == NULL)
		return NULL;
	tree->root = NULL;
	tree->count = 0;
}

/* Seraches TREE for matching ITEM. Retruns 1 if found,
 * 0, otherwise. */
int bin_search(const struct bin_tree * tree, int item)
{
	const struct bin_node *node;
	
	assert(tree != NULL);
	node = tree->root;
	for(;;)
	{
		if (node == NULL)
		{
			return 0;
		}
		else if (item == node->data)
		{
			return 1;
		}
		else if (item > node->data)
		{
			node = node->right;
		}
		else
		{
			node = node->left;
		}
	}
}

/* Inserts Item into TREE. Returns 1 if the item was inserted,
 * 2 if an identical item already existed in Tree, or 0 if a
 * memory allocation error occurred.
 */
int bin_insert(struct bin_tree *tree, int item)
{
	struct bin_node *node, **new;
	
	assert(tree != NULL);
	new = &(tree->root);
	node = tree->root;
	for(;;)
	{		
	
		if(node == NULL)
		{
			node = *new = malloc(sizeof(*node));
			if(node != NULL)
			{
				node->data = item;
				node->left = node->right = NULL;
				tree->count++;
				return 1;
			}
			else
			{
				return 0;
			}	
		}
		else if (item == node->data)
		{
			return 2;
		}
		else if(item > node->data)
		{
			new = &node->right;
			node = node->right;
		}
		else
		{
			new = &node->left;
			node = node->left;
		}
	}
}

/* Delete any item matching ITEM form TREE
 * Returns 1 if such an item was deleted, 
 * if none was found.
 */
int bin_delete(struct bin_tree *tree, int item)
{
	struct bin_node **q, *z;
	
	assert(tree != NULL);
	{
		q = &tree->root;
		z = tree->root;
	}
	for (;;)
	{
		if (z == NULL)
		{
			return 0;
		}
		else if (item == z->data)
		{
			break;
		}
		else if(item > z->data)
		{
			q = &z->right;
			z = z->right;
		}
		else
		{
			q = &z->left;
			z = z->left;
		}
	}
	
	if (z->right == NULL)
	{
		*q = z->left;
	}
	else
	{
		struct bin_node *y = z->right;
		if (y->left == NULL)
		{
			y->left = z->left;
			*q = y;
		}
		else
		{
			struct bin_node *x = y->left;
			while (x->left != NULL)
			{
				y = x;
				x = y->left;
			}
			y->left = x->right;
			x->left = z->left;
			x->right = z->right;
			*q = x;
		}
	}
	
	tree->count--;
	free(z);
	return 1;
}

/* Helper function for bin_walk().Recursively prints 
 * data from each node in tree rooted at NODE in 
 * in-order.
 */
static void walk(const struct bin_node *node)
{
	if (node == NULL)
	{
		return;
	}
	walk(node->left);
	printf("%d ", node->data);
	walk(node->right);
}

/* Prints all the data items in TREE in in-order. */
void bin_walk(const struct bin_tree *tree)
{
	assert(tree != NULL);
	walk(tree->root);
}

/* Prints all the data item in TREE in in-order, 
 * Using an iterative algorithm.
 */
void bin_traverse(const struct bin_tree *tree)
{
	struct bin_node *stack[32];
	int count;
	
	struct bin_node *node;
	
	assert(tree != NULL);
	count = 0;
	node = tree->root;
	for (;;)
	{
		while (node != NULL)
		{
			stack[count++] = node;
			node = node->left;
		}
		if (count == 0)
		{
			return;
		}
		node = stack[--count];
		printf("%d ", node->data);
		node = node->right;
	}
}

/* Destroys tree rooted at NODE. */
static void destory(struct bin_node *node)
{
	if (node == NULL)
	{
		return;
	}
	destory(node->left);
	destory(node->right);
	free(node);
}

/* Destorys TREE.*/
void bin_destroy(struct bin_tree *tree)
{
	assert(tree != NULL);
	destory(tree->root);
	free(tree);
}

/* Returns the number of data in TREE. */
int bin_count(const struct bin_tree *tree)
{
	assert(tree != NULL);
	{
		return tree->count;
	}
}