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
		struct vtree_node **t = vector_at(&p->children, i);
		vtree_sort(*t, cmpfun);
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
		const struct vtree_node **node = cvector_at(&p->children, i);
		vtree_print(*node, print, indent+1);
	}
}

void
vtree_destroy(struct vtree_node *p, void (*freefun)(void *))
{
	for (int i = 0; i < p->children.len; i++) {
		struct vtree_node **n = vector_at(&p->children, i);
		vtree_destroy(*n, freefun);
	}
	vector_destroy(&p->children);
	if (freefun)
		freefun(vtree_container(p));
	return;
}

//okay I gotta test this
