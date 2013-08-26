#include "rb-tree.h"
#include "utils.h"

#include <assert.h>
#include <malloc.h>

/**
 * RB Tree implementation was inspired by Introduction to Algorithms book
 * info: http://www.amazon.com/Introduction-Algorithms-Thomas-H-Cormen/dp/0262033844
 */

struct poco_rb_tree * poco_rbtree_new(bool (*compare)(const void* lhs, const void* rhs),
                                      void (*clear)(const void* private))
{
	struct poco_rb_tree *rb;

	assert(compare);

	if (!(rb = calloc(1, sizeof(struct poco_rb_tree))))
		return NULL;

	rb->compare = compare;
	rb->clear = clear; /* can be NULL */
	rb->count = 0;

	if (!(rb->nil = calloc(1, sizeof(struct poco_rb_node))))
		goto err_nil;

	rb->nil->parent = rb->nil->left = rb->nil->right = rb->nil;
	rb->nil->is_red = false;

	if (!(rb->root = calloc(1, sizeof(struct poco_rb_node))))
		goto err_root;

	rb->root = rb->root->parent = rb->root->left = rb->root->right = rb->nil;
	rb->root->is_red = false;

	return rb;

err_root:
	free(rb->nil);

err_nil:
	free(rb);

	return NULL;
}

struct poco_rb_node * poco_rbtree_node_new(void *private)
{
	struct poco_rb_node *node = NULL;
	if (!(node = calloc(1, sizeof(struct poco_rb_node))))
		return NULL;

	node->private = private;

	return node;
}

void poco_rbtree_free(struct poco_rb_tree *rb)
{
	struct poco_rb_node *node;

	assert(rb && rb->root && rb->nil);

	POCO_RBTREE_FOREACH(node, rb) {
		poco_rbtree_delete(rb, node);
		node = NULL; /* so the for each doesn't break */
	}

	free(rb->root);
	/* we don't need to free nil, since root == nil at this point */
	/* free(rb->nil); */
	free(rb);
}

void rbtree_left_rotate(struct poco_rb_tree *rb, struct poco_rb_node *node)
{
	struct poco_rb_node *x = node->right;
	node->right = x->left;

	if (x->left != rb->nil)
		x->left->parent = node;

	x->parent = node->parent;

	if (node->parent == rb->nil)
		rb->root = x; /* node == rb->root */
	else if (node == node->parent->left)
		node->parent->left = x;
	else
		node->parent->right = x;

	x->left = node;
	node->parent = x;
}

void rbtree_right_rotate(struct poco_rb_tree *rb, struct poco_rb_node *node)
{
	struct poco_rb_node *x = node->left;
	node->left = x->right;

	if (x->right != rb->nil)
		x->right->parent = node;

	x->parent = node->parent;

	if (node->parent == rb->nil)
		rb->root = x; /* node == rb->root */
	else if (node == node->parent->left)
		node->parent->left = x;
	else
		node->parent->right = x;

	x->right = node;
	node->parent = x;
}

bool poco_rbtree_insert(struct poco_rb_tree *rb, struct poco_rb_node *node)
{
	struct poco_rb_node *y;
	struct poco_rb_node *x;

	assert(rb && rb->root && rb->nil);

	y = rb->nil; /* is case x == rb->nil, ie, first node to insert */
	x = rb->root;

	/* normal BST insert */
	while (x != rb->nil) {
		y = x;
		if (rb->compare(node->private, x->private))
			x = x->left;
		else if (rb->compare(x->private, node->private))
			x = x->right;
		else
			return false; /* x == node */
	}

	node->parent = y;

	if (y == rb->nil)
		rb->root = node; /* first node to insert */
	else if (rb->compare(node->private, y->private))
		y->left = node;
	else
		y->right = node;

	node->left = rb->nil;
	node->right = rb->nil;
	node->is_red = true;

	/* RB insert fixup */
	while (node->parent->is_red) {
		if (node->parent == node->parent->parent->left) {
			y = node->parent->parent->right;
			if (y->is_red) {
				node->parent->is_red = false;                    /* Case 1 */
				y->is_red = false;                               /* Case 1 */
				node->parent->parent->is_red = true;             /* Case 1 */
				node = node->parent->parent;                     /* Case 1 */
			} else {
				if (node == node->parent->right) {
					node = node->parent;                         /* Case 2 */
					rbtree_left_rotate(rb, node);                /* Case 2 */
				}
				node->parent->is_red = false;                    /* Case 3 */
				node->parent->parent->is_red = true;             /* Case 3 */
				rbtree_right_rotate(rb, node->parent->parent);   /* Case 3 */
			}
		} else {
			y = node->parent->parent->left;
			if (y->is_red) {
				node->parent->is_red = false;                    /* Case 1 */
				y->is_red = false;                               /* Case 1 */
				node->parent->parent->is_red = true;             /* Case 1 */
				node = node->parent->parent;                     /* Case 1 */
			} else {
				if (node == node->parent->left) {
					node = node->parent;                         /* Case 2 */
					rbtree_right_rotate(rb, node);               /* Case 2 */
				}
				node->parent->is_red = false;                    /* Case 3 */
				node->parent->parent->is_red = true;             /* Case 3 */
				rbtree_left_rotate(rb, node->parent->parent);    /* Case 3 */
			}
		}
	}

	rb->root->is_red = false;
	rb->count++;

	return true;
}

void poco_rbtree_delete(struct poco_rb_tree *rb, struct poco_rb_node *node)
{
	struct poco_rb_node *y;
	struct poco_rb_node *x;
	struct poco_rb_node *w;

	assert(rb && rb->root && rb->nil);

	if (node->left == rb->nil || node->right == rb->nil)
		y = node;
	else
		y = ((w = poco_rbtree_iterate(rb, node))) ? w : rb->nil;

	if (y->left != rb->nil)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;

	if (y->parent == rb->nil)
		rb->root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	/* RB delete fixup */
	if (!y->is_red) {
		while (x != rb->root && !x->is_red) {
			if (x == x->parent->left) {
				w = x->parent->right;
				if (w->is_red) {
					w->is_red = false;                         /* Case 1 */
					x->parent->is_red = true;                  /* Case 1 */
					rbtree_left_rotate(rb, x->parent);         /* Case 1 */
					w = x->parent->right;                      /* Case 1 */
				}
				if (!w->left->is_red && !w->right->is_red) {
					w->is_red = true;                          /* Case 2 */
					x = x->parent;                             /* Case 2 */
				} else {
					if (!w->right->is_red) {
						w->left->is_red = false;               /* Case 3 */
						w->is_red = true;                      /* Case 3 */
						rbtree_right_rotate(rb, w);            /* Case 3 */
						w = x->parent->right;                  /* Case 3 */
					}
					w->is_red = x->parent->is_red;             /* Case 4 */
					x->parent->is_red = false;                 /* Case 4 */
					w->right->is_red = false;                  /* Case 4 */
					rbtree_left_rotate(rb, x->parent);         /* Case 4 */
					x = rb->root;                              /* Case 4 */
				}
			} else {
				w = x->parent->left;
				if (w->is_red) {
					w->is_red = false;                         /* Case 1 */
					x->parent->is_red = true;                  /* Case 1 */
					rbtree_right_rotate(rb, x->parent);        /* Case 1 */
					w = x->parent->left;                       /* Case 1 */
				}
				if (!w->right->is_red && !w->left->is_red) {
					w->is_red = true;                          /* Case 2 */
					x = x->parent;                             /* Case 2 */
				} else {
					if (!w->left->is_red) {
						w->right->is_red = false;              /* Case 3 */
						w->is_red = true;                      /* Case 3 */
						rbtree_left_rotate(rb, w);             /* Case 3 */
						w = x->parent->left;                   /* Case 3 */
					}
					w->is_red = x->parent->is_red;             /* Case 4 */
					x->parent->is_red = false;                 /* Case 4 */
					w->left->is_red = false;                   /* Case 4 */
					rbtree_right_rotate(rb, x->parent);        /* Case 4 */
					x = rb->root;                              /* Case 4 */
				}
			}
		}
		x->is_red = false;
	}

	rb->count--;

	/* if count is < 0, something bad happened */
	assert(rb->count >= 0);

	if (y != node) {
		y->left = node->left;
		y->right = node->right;
		y->parent = node->parent;
		y->is_red = node->is_red;
		node->left->parent = node->right->parent = y;

		if (node == node->parent->left)
			node->parent->left=y;
		else
			node->parent->right=y;
	}

	if (rb->clear)
		rb->clear(node->private);

	free(node);
}

struct poco_rb_node * poco_rbtree_iterate(struct poco_rb_tree *rb, struct poco_rb_node *from_node)
{
	struct poco_rb_node *node;
	struct poco_rb_node *ret = NULL;

	assert(rb && rb->root && rb->nil);

	if (!from_node) {
		/* if from_node is NULL we want to iterate from the begin */
		node = rb->root;
		while (node->left != rb->nil) {
			node = node->left;
		}
		if (node != rb->nil)
			ret = node;
	} else {
		/* find next */

		/* a) from the right */
		if ((node = from_node->right) != rb->nil) {
			while (node->left != rb->nil)
				node = node->left;
			ret = node;
		} else {
			/* b) from the parent */
			/* obs: second usage of node, node is from_node->parent */
			while ((node = from_node->parent) != rb->nil &&
			       node->right == from_node) {
				from_node = from_node->parent;
				node = node->parent;
			}

			if (node != rb->nil)
				ret = node;
		}
	}
	return ret;
}

struct poco_rb_node * poco_rbtree_iterate_backwards(struct poco_rb_tree *rb,
                                                    struct poco_rb_node *from_node)
{
	struct poco_rb_node *node;
	struct poco_rb_node *ret = NULL;

	assert(rb && rb->root && rb->nil);

	if (!from_node) {
		/* if from_node is NULL we want to iterate from the begin */
		node = rb->root;
		while (node->right != rb->nil) {
			node = node->right;
		}
		if (node != rb->nil)
			ret = node;
	} else {
		/* find next */

		/* a) from the left */
		if ((node = from_node->left) != rb->nil) {
			while (node->right != rb->nil)
				node = node->right;
			ret = node;
		} else {
			/* b) from the parent */
			/* obs: second usage of node, node is from_node->parent */
			while ((node = from_node->parent) != rb->nil &&
			       node->left == from_node) {
				from_node = from_node->parent;
				node = node->parent;
			}

			if (node != rb->nil)
				ret = node;
		}
	}
	return ret;
}
