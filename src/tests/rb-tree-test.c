#include "rb-tree.h"

#include <stdlib.h>
#include <time.h>

#include <check.h>

#define NUM_OF_NODES 1000

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#endif

int qsort_comp(const void *lhs, const void *rhs)
{
	return (*(int*)lhs - *(int*)rhs);
}

bool less(const void* lhs, const void* rhs)
{
	int a = (int)lhs;
	int b = (int)rhs;
	return a < b;
}

START_TEST(insertion_check)
{
	struct poco_rb_tree *tree = poco_rbtree_new(less, NULL);
	struct poco_rb_node *node[NUM_OF_NODES], *n;
	int i, r[NUM_OF_NODES];
	bool test;

	srand(time(NULL));

	for (i = 0; i < NUM_OF_NODES; ++i) {
		/* just make sure if we have duplicates we try to generate a non-duplicated private */
		do {
			r[i] = rand() % 100000;
			node[i] = poco_rbtree_node_new((void *)r[i]);
			if (!(test = poco_rbtree_insert(tree, node[i])))
				free(node[i]);
		} while (!test);
	}

	/* sort random values */
	qsort(r, sizeof(r)/sizeof(*r), sizeof(*r), qsort_comp);

	i = 0;
	POCO_RBTREE_FOREACH(n, tree) {
		ck_assert_msg(!!n, "foreach returned NULL node");
		ck_assert_int_eq((int)n->private, r[i++]);
	}

	/* make sure it iterated trough every node */
	ck_assert_int_eq(NUM_OF_NODES, i);

	poco_rbtree_free(tree);
}
END_TEST


START_TEST(insertion_check_backwards)
{
	struct poco_rb_tree *tree = poco_rbtree_new(less, NULL);
	struct poco_rb_node *node[NUM_OF_NODES], *n;
	int i, r[NUM_OF_NODES];
	bool test;

	srand(time(NULL));

	for (i = 0; i < NUM_OF_NODES; ++i) {
		/* just make sure if we have duplicates we try to generate a non-duplicated private */
		do {
			r[i] = rand() % 100000;
			node[i] = poco_rbtree_node_new((void *)r[i]);
			if (!(test = poco_rbtree_insert(tree, node[i])))
				free(node[i]);
		} while (!test);
	}

	/* sort random values */
	qsort(r, sizeof(r)/sizeof(*r), sizeof(*r), qsort_comp);

	i = NUM_OF_NODES - 1;
	POCO_RBTREE_FOREACH_BACKWARDS(n, tree) {
		ck_assert_msg(!!n, "foreach returned NULL node");
		ck_assert_int_eq((int)n->private, r[i--]);
	}

	/* make sure it iterated trough every node */
	ck_assert_int_eq(-1, i);

	poco_rbtree_free(tree);
}
END_TEST

START_TEST(deletion)
{
	struct poco_rb_tree *tree = poco_rbtree_new(less, NULL);
	struct poco_rb_node *node[NUM_OF_NODES], *n;
	int i, r[NUM_OF_NODES];
	bool test;

	srand(time(NULL));

	for (i = 0; i < NUM_OF_NODES; ++i) {
		/* just make sure if we have duplicates we try to generate a non-duplicated private */
		do {
			r[i] = rand() % 100000;
			node[i] = poco_rbtree_node_new((void *)r[i]);
			if (!(test = poco_rbtree_insert(tree, node[i])))
				free(node[i]);
		} while (!test);
	}

	/* sort random values */
	qsort(r, sizeof(r)/sizeof(*r), sizeof(*r), qsort_comp);

	i = 0;
	POCO_RBTREE_FOREACH(n, tree) {
		ck_assert_msg(!!n, "foreach returned NULL node");
		ck_assert_int_eq((int)n->private, r[i++]);
		poco_rbtree_delete(tree, n);
		n = NULL; /* so the for each doesn't break */
	}

	/* make sure it iterated trough every node */
	ck_assert_int_eq(tree->count, 0);

	poco_rbtree_free(tree);
}
END_TEST


START_TEST(foreach_empty)
{
	struct poco_rb_tree *tree = poco_rbtree_new(less, NULL);
	struct poco_rb_node *n;

	POCO_RBTREE_FOREACH(n, tree)
		ck_assert_msg(true, "foreach shouldn't iterate at all");

	poco_rbtree_free(tree);
}
END_TEST

START_TEST(foreach_backwards_empty)
{
	struct poco_rb_tree *tree = poco_rbtree_new(less, NULL);
	struct poco_rb_node *n;

	POCO_RBTREE_FOREACH_BACKWARDS(n, tree)
		ck_assert_msg(true, "foreach shouldn't iterate at all");

	poco_rbtree_free(tree);
}
END_TEST


Suite *rbtree_suite(void)
{
	Suite *s = suite_create("Red-black Tree");

	/* Core test case */
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, insertion_check);
	tcase_add_test(tc_core, insertion_check_backwards);
	tcase_add_test(tc_core, deletion);
	tcase_add_test(tc_core, foreach_empty);
	tcase_add_test(tc_core, foreach_backwards_empty);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(int argc, char *argv[])
{
	int number_failed;
	Suite *s = rbtree_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
