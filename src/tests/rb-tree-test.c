#include "rb-tree.h"

#include <stdlib.h>

#include <check.h>

#define NUM 100

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#endif

bool less(const void* lhs, const void* rhs)
{
	int a = (int)lhs;
	int b = (int)rhs;
	return a < b;
}

START_TEST(foreach)
{
	struct poco_rb_tree *tree = poco_rbtree_new(less, NULL);
	struct poco_rb_node *node[NUM];
	struct poco_rb_node *n;
	int i;
	for (i = 0; i < NUM; ++i) {
		node[i] = poco_rbtree_node_new((void *)i);
		poco_rbtree_insert(tree, node[i]);
	}

	i = 0;
	POCO_RBTREE_FOREACH(n, tree) {
		ck_assert_msg(!!n, "foreach returned NULL node");
		ck_assert_int_eq((int)n->private, i++);
	}

	/* make sure it iterated trough every node */
	ck_assert_int_eq(NUM, i);

	/* poco_rbtree_free(tree); */
}
END_TEST

START_TEST(foreach_backwards)
{
	struct poco_rb_tree *tree = poco_rbtree_new(less, NULL);
	struct poco_rb_node *node[NUM];
	struct poco_rb_node *n;
	int i;
	for (i = 0; i < NUM; ++i) {
		node[i] = poco_rbtree_node_new((void *)i);
		poco_rbtree_insert(tree, node[i]);
	}

	i = NUM - 1;
	POCO_RBTREE_FOREACH_BACKWARDS(n, tree) {
		ck_assert_msg(!!n, "backwards foreach returned NULL node");
		ck_assert_int_eq((int)n->private, i--);
	}

	/* make sure it iterated trough every node */
	ck_assert_int_eq(-1, i);

	/* poco_rbtree_free(tree); */
}
END_TEST

Suite *rbtree_suite(void)
{
	Suite *s = suite_create("Red-black Tree");

	/* Core test case */
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, foreach);
	tcase_add_test(tc_core, foreach_backwards);
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
