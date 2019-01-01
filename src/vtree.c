#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "stack.h"
#include "tree.h"

void *
vtree_container(struct vtree_node *p)
{
	return (char *)p - p->offset;
}

void
vtree_node_init(struct vtree_node *n, unsigned int offset)
{
	n->offset = offset;
	vector_init_zero(&n->children, sizeof(struct vtree_node *), NULL);
	n->parent = NULL;
}




/*
 * we have one option, sort p and all it's desendens
 */
void
vtree_sort(struct vtree_node *p, int (*cmpfun)(const void *, const void *))
{
	if (p->children.len == 0)
		return;
	qsort(p->children.elems, p->children.len, p->children.elemsize, cmpfun);
	for (int i = 0; i < p->children.len; i++) {
		struct vtree_node *t = vector_at(&p->children, i);
		vtree_sort(t, cmpfun);
	}
}

struct vtree_node *
vtree_find(struct vtree_node *p, struct vtree_node *key,
	   int (*cmpfun)(const void *, const void *))
{
	//you can uses the same compare function in sort
	return bsearch(key, p->children.elems,
		       p->children.len, p->children.elemsize,
		       cmpfun);
}

void
vtree_print(const struct vtree_node *p,
	    void (*print)(const struct vtree_node *), int indent)
{
	for (int i = 0; i < indent; i++)
		fprintf(stderr, "\t");
	print(p);

	for (int i = 0; i < p->children.len; i++) {
		const struct vtree_node *node = cvector_at(&p->children, i);
		vtree_print(node, print, indent+1);
	}
}

void
vtree_destroy(struct vtree_node *p, void (*freefun)(void *))
{
	//leaf, there is nothing we do here
	if (p->children.len == 0) {
		if (freefun)
			freefun(vtree_container(p));
		return;
	}
	else {
		for (int i = 0; i < p->children.len; i++) {
			vtree_destroy((struct vtree_node *)vector_at(&p->children, i),
				freefun);
		}
		vector_destroy(&p->children);
	}
}

//okay I gotta test this
/*
struct test_tree {
	int val;
	struct vtree_node node;
};

int compare_test_tree(const void *a, const void *b)
{
	struct vtree_node *l = deref(struct vtree_node **, a);
	struct vtree_node *r = deref(struct vtree_node **, b);
	struct test_tree *lt = container_of(l, struct test_tree, node);
	struct test_tree *rt = container_of(r, struct test_tree, node);
	return (lt->val == rt->val) ? 0 : (lt->val > rt->val) ? 1 : -1;
	return lt->val > rt->val;
}

int compare_key_tree(const void *key, const void *b)
{
	int a = *(int *)key;
	struct vtree_node *r = deref(struct vtree_node **, b);
	struct test_tree *rt = container_of(r, struct test_tree, node);
//	printf("%d %d\n", a, rt->val);
	return (a == rt->val) ? 0 : (a > rt->val) ? 1 : -1;
//	return a > rt->val;
}

int main(int argc, char *argv[])
{
	struct test_tree test = {
		.val = 51,
		.node = {0},
	};
	vtree_node_init(&test.node);
	//create a tree
	for (int i = 0; i < 50; i++) {
		struct test_tree *tmp = (struct test_tree *)xmalloc(sizeof(struct test_tree));
		tmp->val = i;
		vtree_node_init(&tmp->node);
		vtree_node_add_child(&test.node, &tmp->node);
	}
	for (int i = 52; i < 100; i++) {
		struct test_tree *tmp = (struct test_tree *)xmalloc(sizeof(struct test_tree));
		tmp->val = i;
		vtree_node_init(&tmp->node);
		vtree_node_add_child(&test.node, &tmp->node);
	}
	int b = 10;
	vtree_sort(&test.node, compare_test_tree);
//	for (int i = 0; i < 98; i++) {
//		struct vtree_node *tmp = deref(struct vtree_node**,
//					      (char *)test.node.children.elems + test.node.children.elemsize * i);
//		struct test_tree *t = container_of(tmp, struct test_tree, node);
//		printf("%d\n", t->val);
//	}
//	vtree_find(&test.node, &b, compare_key_tree);
	return 0;
}
*/
