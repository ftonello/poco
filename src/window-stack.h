#ifndef _WINDOW_STACK_H_
#define _WINDOW_STACK_H_

#include "rb-tree.h"

struct poco_window_stack_node {
	struct poco_rb_node *node;
	struct poco_window *window;
};

#endif /* _WINDOW-STACK_H_ */
