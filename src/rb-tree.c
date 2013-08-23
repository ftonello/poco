#include "rb-tree.h"

#include <assert.h>
#include <malloc.h>

poco_rb_tree * poco_rbtree_new(bool (*compare)(const void* lhs, const void* rhs),
                               void (*clear)(const void* private))
{
	struct poco_rb_tree *rb;

	assert(compare);

	if (!(rb = calloc(1, sizeof(struct poco_rb_tree))))
		return NULL;

	rb->compare = compare;
	rb->clear = clear; /* can be NULL */

	if (!(rb->root = calloc(1, sizeof(struct poco_rb_node))))
		goto err;

	rb->root->parent = rb->root->left = rb->root->right = NULL;
	rb->root->is_red = false;
	rb->root->private = NULL;

	return rb;

err:
	free(rb);

	return NULL;
}

void poco_rbtree_free(poco_rb_tree *rb)
{
	struct poco_rb_node *node;

	assert(rb);

	POCO_RBTREE_FOREACH(node, rb) {
		if (rb->clear)
			rb->clear(node->private);
		free(node);
	}

	free(rb->root);
	free(rb);
}

void rbtree_right_rotate(struct poco_rb_tree *rb, struct poco_rb_node *node)
{
	struct poco_rb_node *x = node->right;
	node->right = x->left;

	if (x->left)
		x->left->parent = node;

	x->parent = node->parent;

	if (!node->parent)
		rb->root = x; /* node == rb->root */
	else if (node == node->parent->left)
		node->parent->left = x;
	else
		node->parent->right = x;

	x->left = node;
	node->parent = x;
}

void rbtree_left_rotate(struct poco_rb_tree *rb, struct poco_rb_node *node)
{
	struct poco_rb_node *x = node->left;
	node->left = x->right;

	if (x->right)
		x->right->parent = node;

	x->parent = node->parent;

	if (!node->parent)
		rb->root = x; /* node == rb->root */
	else if (node == node->parent->left)
		node->parent->left = x;
	else
		node->parent->right = x;

	x->right = node;
	node->parent = x;
}

void poco_rbtree_insert(struct poco_rb_tree *rb, struct poco_rb_node *node)
{
	struct poco_rb_node *y = NULL;
	struct poco_rb_node *x;

	assert(rb);
	assert(rb->root);

	x = rb->root;

	/* normal BST insert */
	while (x) {
		y = x;
		if (rb->compare(node->private, x->private)) {
			if (!x->left)
				x->left = node;
			x = x->left;
		}else {
			if (!x->right)
				x->right = node;
			x = x->right;
		}
	}

	node->parent = y;
	node->left = NULL;
	node->right = NULL;
	node->is_red = true;

	/* RB fixup */
	/* TODO: rest of the function */
}

void poco_rbtree_delete(struct poco_rb_tree *rb, struct poco_rb_node *node)
{
	assert(rb);

}

struct poco_rb_node * poco_rbtree_iterate(struct poco_rb_tree *rb, struct poco_rb_node *from_node)
{
	struct poco_rb_node *node;

	assert(rb);

	if (!from_node) {
		/* if from_node is NULL we want to iterate from the begin */
		node = rb->root;
		while (node->left) {
			node = node->left;
		}
		return node;
	} else {
		if (from_node->right)
			return from_node->right;

		if ((node = from_node->parent)) {
			/* node is parent */
			if (node->right == from_node) {
				/* we need grand-parent */
				if (node->parent)
					return node->parent;
			} else {
				return node;
			}
		}

		/* if reached the end */
		return NULL;
	}
}

struct poco_rb_node * poco_rbtree_iterate_backwards(struct poco_rb_tree *rb,
                                                    struct poco_rb_node *from_node)
{
	struct poco_rb_node *node;

	assert(rb);

	if (!from_node) {
		/* if from_node is NULL we want to iterate from the end */
		node = rb->root;
		while (node->right) {
			node = node->right;
		}
		return node;
	} else {

		if (from_node->left)
			return from_node->left;

		if ((node = from_node->parent)) {
			/* node is parent */
			if (node->left == from_node) {
				/* we need grand-parent */
				if (node->parent)
					return node->parent;
			} else {
				return node;
			}
		}

		/* if not found */
		return NULL;
	}
}
