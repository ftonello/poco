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
	bool (*compare)(const void* lhs, const void* rhs);
	void (*clear)(const void* private);
	struct poco_rb_node *root;
	/* we don't need to have a nil node because we don't need to count the height */
};

struct poco_rb_tree * poco_rbtree_new(bool (*compare)(const void* lhs, const void* rhs),
                                      void (*clear)(const void* private));
void poco_rbtree_free(struct poco_rb_tree *rb);
void poco_rbtree_insert(struct poco_rb_tree *rb, struct poco_rb_node *node);
void poco_rbtree_delete(struct poco_rb_tree *rb, struct poco_rb_node *node);

/* if from_node is NULL, it means that it should iterate from the begin/end */
struct poco_rb_node * poco_rbtree_iterate(struct poco_rb_tree *rb, struct poco_rb_node *from_node);
struct poco_rb_node * poco_rbtree_iterate_backwards(struct poco_rb_tree *rb,
                                                    struct poco_rb_node *from_node);

#define POCO_RBTREE_FOREACH(node, rb) \
	for ((node) = poco_rbtree_iterate((rb), NULL); (node); (node) = poco_rbtree_iterate((rb), (node)))

#define POCO_RBTREE_FOREACH_BACKWARDS(node, rb) \
	for ((node) = poco_rbtree_iterate_backwards((rb), NULL); (node); (node) = poco_rbtree_iterate_backwards((rb), (node)))

#endif /* _RB-TREE_H_ */
