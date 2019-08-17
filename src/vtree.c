#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "stack.h"
#include "tree.h"
#include "queue.h"

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
		struct vtree_node **t = vector_at(&p->children, i);
		vtree_sort(*t, cmpfun);
	}
}

void
vtree_node_shift(struct vtree_node *v, bool forward)
{
	if (!v->parent)
		return;
	int index = 0;
	for (int i = 0; i < v->parent->children.len; i++) {
		if (vtree_ith_child(v->parent, i) == v) {
			index = i;
			break;
		}
	}
	int step = forward ? 1 : -1;
	if (index+step < 0 || index + step >= v->parent->children.len)
		return;
	//swap the element
	struct vtree_node **self =
		(struct vtree_node **)vector_at(&v->parent->children, index);
	struct vtree_node **another =
		(struct vtree_node **)vector_at(&v->parent->children, index + step);
	struct vtree_node *tmp = *another;
	*another = *self;
	*self = tmp;
}

off_t
vtree_find(struct vtree_node *p, const void *data,
	   int (*cmpfun)(const void *, const struct vtree_node *))
{
	for (int i = 0; i < p->children.len; i++) {
		struct vtree_node *n = vtree_ith_child(p, i);
		if (!cmpfun(data, n))
			return i;
	}
	return -1;
}

void
vtree_print(const struct vtree_node *p,
	    void (*print)(const struct vtree_node *), int indent)
{
	for (int i = 0; i < indent; i++)
		fprintf(stderr, "\t");
	print(p);

	for (int i = 0; i < p->children.len; i++) {
		struct vtree_node *node = vtree_ith_child((struct vtree_node *)p, i);
		vtree_print(node, print, indent+1);
	}
}

void
vtree_destroy(struct vtree_node *p, void (*freefun)(void *))
{
	for (int i = 0; i < p->children.len; i++) {
		struct vtree_node **n = vector_at(&p->children, i);
		if (*n)
			vtree_destroy(*n, freefun);
	}
	vector_destroy(&p->children);
	if (freefun)
		freefun(vtree_container(p));
	return;
}

void
vtree_destroy_children(struct vtree_node *p, void (*freefun)(void *))
{
	for (int i = 0; i < p->children.len; i++) {
		struct vtree_node **n = vector_at(&p->children, i);
		if (*n)
			vtree_destroy(*n, freefun);
	}
	vector_destroy(&p->children);
}


int
vtree_iterate(const struct vtree_node *root, void *data,
		  void (*visit)(const struct vtree_node *, void *))
{
	int count = 0;
	cstack_t stack;
	cstack_init(&stack, sizeof(struct vtree_node *), NULL);
	cstack_append(&stack, &root);
	while (!cstack_empty(&stack)) {
		struct vtree_node *node = *(struct vtree_node **)cstack_top(&stack);
		cstack_pop(&stack);
		count += 1;
		visit(node, data);
		for (int i = 0; i < node->children.len; i++) {
			struct vtree_node *n = vtree_ith_child(node, i);
			/* printf("%p\n", n); */
			cstack_append(&stack, &n);
		}
	}
	return count;
}


struct vtree_node *
vtree_search(const struct vtree_node *root, void *data,
	     int (*cmpfun)(const void *, const struct vtree_node *))
{
	struct vtree_node *node = NULL;
	//we do the BFS search
	queue_t queue;
	queue_init(&queue, sizeof(struct vtree_node *), NULL);
	queue_append(&queue, &root);
	while (!queue_empty(&queue)) {
		struct vtree_node *curr = *(struct vtree_node **)
			queue_top(&queue);
		queue_pop(&queue);
		if (!cmpfun(data, curr)) {
			node = curr;
			break;
		}
		for (int i = 0; i < curr->children.len; i++) {
			struct vtree_node *n = vtree_ith_child(curr, i);
			queue_append(&queue, &n);
		}
	}
	queue_destroy(&queue);
	return node;
}
