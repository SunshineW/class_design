#ifndef BIN_H_
#define BIN_H_

/* A binary tree node.*/
struct bin_node
{
	int data;
	struct bin_node *left;
	struct bin_node *right;
};

/* A binary tree. */
struct bin_tree
{
	struct bin_node *root;
	int count;
};

/* Creates and returns a new binary tree.
 * Returns a null  pointer ig a memory 
 * allocation error occurs. 
 */
struct bin_tree *bin_create(void);

/* Seraches TREE for matching ITEM. Retruns 1 if found,
 * 0, otherwise. */
int bin_search(const struct bin_tree * tree, int item);

/* Inserts Item into TREE. Returns 1 if the item was inserted,
 * 2 if an identical item already existed in Tree, or 0 if a
 * memory allocation error occurred.
 */
int bin_insert(struct bin_tree *tree, int item);

/* Delete any item matching ITEM form TREE
 * Returns 1 if such an item was deleted, 
 * 0 if none was found.
 */
int bin_delete(struct bin_tree *tree, int item);

/* Helper function for bin_walk().Recursively prints 
 * data from each node in tree rooted at NODE in 
 * in-order.
 */
static void walk(const struct bin_node *node);

/* Prints all the data items in TREE in in-order. */
void bin_walk(const struct bin_tree *tree);

/* Prints all the data item in TREE in in-order, 
 * Using an iterative algorithm.
 */
void bin_traverse(const struct bin_tree *tree);

/* Destroys tree rooted at NODE. */
static void destory(struct bin_node *node);

/* Destorys TREE.*/
void bin_destroy(struct bin_tree *tree);

/* Returns the number of data in TREE. */
int bin_count(const struct bin_tree *tree);

#endif

