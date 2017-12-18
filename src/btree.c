#include <stdlib.h>

#include "vector.h"

struct btree_node {
	//we can take the advantage of the sorting and search
	vector_t children;
	struct btree_node *parent;
};

#define btree_ptr_size sizeof(struct btree_node *)

void
btree_node_init(struct btree_node *n)
{
	n->children.len = sizeof(struct btree_node *);
	n->children.alloc_len = 0;
	n->children.len = 0;
	n->children.elems = NULL;
}

void
btree_node_add_child(struct btree_node* p, struct btree_node *c)
{
	if (c->children.len != btree_ptr_size) {
		//leap node
		btree_node_init(c);
	} else {
		deref(struct btree_node **,
		      vector_newelem(&p->children)) = c;
	}
}

/*
 * we have one option, sort p and all it's desendens
 */
void
btree_sort(struct btree_node *p, int (*cmpfun)(const void *, const void *))
{
	if (p->children.len == 0)
		return;
	qsort(p->children.elems, p->children.len, p->children.elemsize, cmpfun);
	int i;
	for (i = 0; i < p->children.len; i++) {
		struct btree_node *t = deref(struct btree_node **,
					     vector_at(&p->children, i));
		btree_sort(t, cmpfun);
	}
}

struct btree_node *
btree_find(struct btree_node *p, void *key,
	   int (*cmpfun)(const void *, const void *))
{
	//cmp function has to be cmp (key_t *key, btree_node *t) or btree_node **t
	return *(struct btree_node **)bsearch(key, p->children.elems, p->children.len, p->children.elemsize, cmpfun);
}


//okay I gotta test this
struct test_tree {
	int val;
	struct btree_node node;
};


int main(int argc, char *argv[])
{
	struct test_tree test = {
		.val = 51,
		.node = {0},
	};
	btree_node_init(&test.node);
	//create a tree
	for (int i = 0; i < 50; i++) {
		struct test_tree *tmp = (struct test_tree *)xmalloc(sizeof(struct test_tree));
		tmp->val = i;
		btree_node_init(&tmp->node);
		btree_node_add_child(&test.node, &tmp->node);
	}
	for (int i = 52; i < 100; i++) {
		struct test_tree *tmp = (struct test_tree *)xmalloc(sizeof(struct test_tree));
		tmp->val = i;
		btree_node_init(&tmp->node);
		btree_node_add_child(&test.node, &tmp->node);
	}
	return 0;
}
