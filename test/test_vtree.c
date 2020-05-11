#include <stdio.h>
#include <stdlib.h>
#include <ctypes/helpers.h>
#include <ctypes/tree.h>


struct test_tree {
	int val;
	struct vtree_node node;
};

int compare_test_tree(const void *a, const void *b)
{
	const struct test_tree *lt = container_of((const struct vtree_node *)a, const struct test_tree, node);
	const struct test_tree *rt = container_of((const struct vtree_node *)b, const struct test_tree, node);
	return lt->val - rt->val;
}

int compare_key_tree(const void *key, const void *b)
{
	const struct test_tree *key_tree = container_of((const struct vtree_node *)key, const struct test_tree, node);
	const struct test_tree *rt = container_of((const struct vtree_node *)b, const struct test_tree, node);
//	printf("%d %d\n", a, rt->val);
	return key_tree->val - rt->val;
//	return a > rt->val;
}

static void
randomly_add_child(struct vtree_node *root, int val)
{
	off_t offset = offsetof(struct test_tree, node);

	struct vtree_node *subtree = root;
	for (int i = 0; i < 10; i++) {
		if (!subtree->children.len)
			break;
		int j = ABS(rand()) % subtree->children.len;
		struct vtree_node *n = vtree_ith_child(subtree, j);
		bool append = ABS(rand()) % 2 > 0;
		if (append)
			break;
		else
			subtree = n;
	}
	struct test_tree *tmp = (struct test_tree *)xmalloc(sizeof(struct test_tree));
	tmp->val = val;
	vtree_node_init(&tmp->node, offset);
	vtree_node_add_child(subtree, &tmp->node);
}

/*
static void print_node(const struct vtree_node *n) {
	const struct test_tree *tree = container_of(n, const struct test_tree, node);
	fprintf(stderr, "%d\n", tree->val);
}
*/

static void print_node1(const struct vtree_node *n, void *data)
{
	const struct test_tree *tree = container_of(n, const struct test_tree, node);
	fprintf(stderr, "%d\n", tree->val);
}

static int
search_number(const void *d, const struct vtree_node *node)
{
	int data = (long)d;
	const struct test_tree *tree = container_of(node, const struct test_tree, node);
	if (tree->val == data)
		return 0;
	return -1;
}


int main(int argc, char *argv[])
{
	off_t offset = offsetof(struct test_tree, node);

	struct test_tree *test = xmalloc(sizeof(struct test_tree));
	vtree_node_init(&test->node, offset);
	//sequential test
	/* for (int i = 0; i < 50; i++) { */
	/*	struct test_tree *tmp = (struct test_tree *)xmalloc(sizeof(struct test_tree)); */
	/*	tmp->val = i; */
	/*	vtree_node_init(&tmp->node, offset); */
	/*	vtree_node_add_child(&test->node, &tmp->node); */
	/* } */
	/* for (int i = 52; i < 100; i++) { */
	/*	struct test_tree *tmp = (struct test_tree *)xmalloc(sizeof(struct test_tree)); */
	/*	tmp->val = i; */
	/*	vtree_node_init(&tmp->node, offset); */
	/*	vtree_node_add_child(&test->node, &tmp->node); */
	/* } */
	/* vtree_sort(&test->node, compare_test_tree); */
	/* vtree_destroy(&test->node, free); */

	//create a tree
	for (int i = 0; i < 50; i++)
		randomly_add_child(&test->node, i);
	/* vtree_print(&test->node, print_node, 0); */
	vtree_iterate(&test->node, NULL, print_node1);
	struct vtree_node *result = vtree_search(&test->node, (void *)32, search_number);
	struct test_tree *tr = container_of(result, struct test_tree, node);

	ASSERT(tr->val == 32);

	/* int b = 10; */
//	for (int i = 0; i < 98; i++) {
//		struct vtree_node *tmp = deref(struct vtree_node**,
//					      (char *)test.node.children.elems + test.node.children.elemsize * i);
//		struct test_tree *t = container_of(tmp, struct test_tree, node);
//		printf("%d\n", t->val);
//	}
//	vtree_find(&test.node, &b, compare_key_tree);
	vtree_destroy(&test->node, free);

	return 0;
}
