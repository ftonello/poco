#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include <stdbool.h>

struct poco_rb_node {
	struct poco_rb_node *parent;
	struct poco_rb_node *left;
	struct poco_rb_node *right;
	void *private;
	bool is_red;
};

struct poco_rb_tree {
	bool (*less)(const void* a, const void* b);
	void (*clear)(const void* private);
	struct poco_rb_node *root;
	/* we don't need to have a nil node because we don't need to count the height */
};

struct poco_rb_tree * poco_rbtree_new(bool (*less)(const void* lhs, const void* rhs),
                                      void (*clear)(const void* private));
void poco_rbtree_free(poco_rb_tree *rb);
void poco_rbtree_insert(poco_rb_tree *rb, poco_rb_node *node);
void poco_rbtree_delete(poco_rb_tree *rb, poco_rb_node *node);

/* if from_node is NULL, it means that it should iterate from the begin/end */
struct poco_rb_node * poco_rbtree_iterate(poco_rb_tree *rb, poco_rb_node *from_node);
struct poco_rb_node * poco_rbtree_iterate_backwards(poco_rb_tree *rb, poco_rb_node *from_node);

#define POCO_RBTREE_FOREACH(node, rb) \
	for ((node) = poco_rbtree_iterate((rb), NULL); (node); (node) = poco_rbtree_iterate((rb), (node)))

#define POCO_RBTREE_FOREACH_BACKWARDS(node, rb) \
	for ((node) = poco_rbtree_iterate_backwards((rb), NULL); (node); (node) = poco_rbtree_iterate_backwards((rb), (node)))

#endif /* _RB-TREE_H_ */
