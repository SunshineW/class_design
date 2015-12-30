#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* An AVL tree node. */
struct avl_node {
  struct avl_node *link[2];
  int data;
  short bal;
};

/* An AVL tree. */
struct avl_tree {
  struct avl_node *root;
  int count;
};

/* Creates and returns a new AVL tree.  Returns a null pointer if a
   memory allocation error occurs. */
struct avl_tree *avl_create(void)
{
  struct avl_tree *tree = malloc(sizeof *tree);
  if (tree == NULL)
    return NULL;
  tree->root = NULL;
  tree->count = 0;
  return tree;
}

/* Searches TREE for matching ITEM.  Returns 1 if found, 0
   otherwise. */
int avl_search(const struct avl_tree *tree, int item)
{
  const struct avl_node *node;
  assert(tree != NULL);
  node = tree->root;
  for (;;) {
    if (node == NULL)
      return 0;
    else if (item == node->data)
      return 1;
    else if (item > node->data)
      node = node->link[1];
    else
      node = node->link[0];
  }
}

/* Allocates a new node in TREE at *NODE.  Sets the node's parent to
   PARENT and data to ITEM, and initializes the other fields
   appropriately. */
static struct avl_node *new_node(struct avl_tree *tree, int item)
{
  struct avl_node *node = malloc(sizeof *node);
  if (node == NULL)
    return NULL;
  node->data = item;
  node->link[0] = node->link[1] = NULL;
  node->bal = 0;
  tree->count++;
  return node;
}

/* Inserts ITEM into TREE.  Returns 1 if the item was inserted, 2 if
   an identical item already existed in TREE, or 0 if a memory
   allocation error occurred. */
int avl_insert(struct avl_tree *tree, int item)
{
  struct avl_node **v, *w, *x, *y, *z;

  assert(tree != NULL);
  v = &tree->root;
  x = z = tree->root;
  if (x == NULL) {
    tree->root = new_node(tree, item);
    return tree->root != NULL;
  }

  for (;;) {
    int dir;
    if (item == z->data)
      return 2;

    dir = item > z->data;
    y = z->link[dir];
    if (y == NULL) {
      y = z->link[dir] = new_node(tree, item);
      if (y == NULL)
	return 0;
      break;
    }

    if (y->bal != 0) {
      v = &z->link[dir];
      x = y;
    }
    z = y;
  }

  w = z = x->link[item > x->data];
  while (z != y)
    if (item < z->data) {
      z->bal = -1;
      z = z->link[0];
    }
    else {
      z->bal = +1;
      z = z->link[1];
    }

  if (item < x->data) {
    if (x->bal != -1)
      x->bal--;
    else if (w->bal == -1) {
      *v = w;
      x->link[0] = w->link[1];
      w->link[1] = x;
      x->bal = w->bal = 0;
    }
    else {
      assert(w->bal == +1);
      *v = z = w->link[1];
      w->link[1] = z->link[0];
      z->link[0] = w;
      x->link[0] = z->link[1];
      z->link[1] = x;
      if (z->bal == -1) {
	x->bal = 1;
	w->bal = 0;
      }
      else if (z->bal == 0)
	x->bal = w->bal = 0;
      else {
	assert(z->bal == +1);
	x->bal = 0;
	w->bal = -1;
      }
      z->bal = 0;
    }
  }
  else {
    if (x->bal != +1)
      x->bal++;
    else if (w->bal == +1) {
      *v = w;
      x->link[1] = w->link[0];
      w->link[0] = x;
      x->bal = w->bal = 0;
    }
    else {
      assert(w->bal == -1);
      *v = z = w->link[0];
      w->link[0] = z->link[1];
      z->link[1] = w;
      x->link[1] = z->link[0];
      z->link[0] = x;
      if (z->bal == +1) {
	x->bal = -1;
	w->bal = 0;
      }
      else if (z->bal == 0)
	x->bal = w->bal = 0;
      else {
	assert(z->bal == -1);
	x->bal = 0;
	w->bal = 1;
      }
      z->bal = 0;
    }
  }
  return 1;
}

/* Deletes any item matching ITEM from TREE.  Returns 1 if such an
   item was deleted, 0 if none was found. */
int avl_delete(struct avl_tree *tree, int item)
{
  struct avl_node *ap[32];
  int ad[32];
  int k = 1;

  struct avl_node **y, *z;

  assert(tree != NULL);

  ad[0] = 0;
  ap[0] = (struct avl_node *) &tree->root;

  z = tree->root;
  for (;;) {
    int dir;
    if (z == NULL)
      return 0;
    if (item == z->data)
      break;

    dir = item > z->data;
    ap[k] = z;
    ad[k++] = dir;
    z = z->link[dir];
  }

  tree->count--;
  y = &ap[k - 1]->link[ad[k - 1]];
  if (z->link[1] == NULL)
    *y = z->link[0];
  else {
    struct avl_node *x = z->link[1];
    if (x->link[0] == NULL) {
      x->link[0] = z->link[0];
      *y = x;
      x->bal = z->bal;
      ad[k] = 1;
      ap[k++] = x;
    }
    else {
      struct avl_node *w = x->link[0];
      int j = k++;

      ad[k] = 0;
      ap[k++] = x;
      while (w->link[0] != NULL) {
	x = w;
	w = x->link[0];
	ad[k] = 0;
	ap[k++] = x;
      }

      ad[j] = 1;
      ap[j] = w;
      w->link[0] = z->link[0];
      x->link[0] = w->link[1];
      w->link[1] = z->link[1];
      w->bal = z->bal;
      *y = w;
    }
  }

  free(z);
  assert(k > 0);
  while (--k) {
    struct avl_node *w, *x;

    w = ap[k];
    if (ad[k] == 0) {
      if (w->bal == -1) {
	w->bal = 0;
	continue;
      }
      else if (w->bal == 0) {
	w->bal = 1;
	break;
      }

      assert(w->bal == +1);

      x = w->link[1];
      assert(x != NULL);

      if (x->bal > -1) {
	w->link[1] = x->link[0];
	x->link[0] = w;
	ap[k - 1]->link[ad[k - 1]] = x;
	if (x->bal == 0) {
	  x->bal = -1;
	  break;
	}
	else
	  w->bal = x->bal = 0;
      }
      else {
	assert(x->bal == -1);
	z = x->link[0];
	x->link[0] = z->link[1];
	z->link[1] = x;
	w->link[1] = z->link[0];
	z->link[0] = w;
	if (z->bal == +1) {
	  w->bal = -1;
	  x->bal = 0;
	}
	else if (z->bal == 0)
	  w->bal = x->bal = 0;
	else {
	  assert(z->bal == -1);
	  w->bal = 0;
	  x->bal = +1;
	}
	z->bal = 0;
	ap[k - 1]->link[ad[k - 1]] = z;
      }
    }
    else {
      assert(ad[k] == 1);
      if (w->bal == +1) {
	w->bal = 0;
	continue;
      }
      else if (w->bal == 0) {
	w->bal = -1;
	break;
      }

      assert(w->bal == -1);

      x = w->link[0];
      assert(x != NULL);

      if (x->bal < +1) {
	w->link[0] = x->link[1];
	x->link[1] = w;
	ap[k - 1]->link[ad[k - 1]] = x;
	if (x->bal == 0) {
	  x->bal = +1;
	  break;
	}
	else
	  w->bal = x->bal = 0;
      }
      else if (x->bal == +1) {
	z = x->link[1];
	x->link[1] = z->link[0];
	z->link[0] = x;
	w->link[0] = z->link[1];
	z->link[1] = w;
	if (z->bal == -1) {
	  w->bal = 1;
	  x->bal = 0;
	}
	else if (z->bal == 0)
	  w->bal = x->bal = 0;
	else {
	  assert(z->bal == 1);
	  w->bal = 0;
	  x->bal = -1;
	}
	z->bal = 0;
	ap[k - 1]->link[ad[k - 1]] = z;
      }
    }
  }

  return 1;
}

/* Helper function for avl_walk().  Recursively prints data from each
   node in tree rooted at NODE in in-order. */
static void walk(const struct avl_node *node)
{
  if (node == NULL)
    return;
  walk(node->link[0]);
  printf("%d ", node->data);
  walk(node->link[1]);
}

/* Prints all the data items in TREE in in-order. */
void avl_walk(const struct avl_tree *tree)
{
  assert(tree != NULL);
  walk(tree->root);
}

/* Prints all the data items in TREE in in-order, using an iterative
   algorithm. */
void avl_traverse(const struct avl_tree *tree)
{
  struct avl_node *stack[32];
  int count;
  struct avl_node *node;

  assert(tree != NULL);
  count = 0;
  node = tree->root;
  for (;;) {
    while (node != NULL) {
      stack[count++] = node;
      node = node->link[0];
    }
    if (count == 0)
      return;

    node = stack[--count];
    printf("%d ", node->data);
    node = node->link[1];
  }
}

/* Destroys tree rooted at NODE. */
static void destroy(struct avl_node *node)
{
  if (node == NULL)
    return;
  destroy(node->link[0]);
  destroy(node->link[1]);
  free(node);
}

/* Destroys TREE. */
void avl_destroy(struct avl_tree *tree)
{
  assert(tree != NULL);
  destroy(tree->root);
  free(tree);
}

/* Returns the number of data items in TREE. */
int avl_count(const struct avl_tree *tree)
{
  assert(tree != NULL);
  return tree->count;
}

/* Arrange the N elements of ARRAY in random order. */
void shuffle(int *array, int n)
{
  int i;
  for (i = 0; i < n; i++) {
    int j = i + rand() % (n - i);
    int t = array[j];
    array[j] = array[i];
    array[i] = t;
  }
}