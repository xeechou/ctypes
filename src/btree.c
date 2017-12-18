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
		*ptr_derefer(struct btree_node *, vector_newelem(&p->children)) = c;
	}
}
