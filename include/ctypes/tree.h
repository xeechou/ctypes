/*
 * Copyright (c) 2019 Xichen Zhou
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CTYPES_TREE_H
#define CTYPES_TREE_H

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief a variable length tree node implementation.
 *
 * It is not very efficient since each node holds a piece of memory, it is not a
 * search tree, so the struture of tree reflects the structure of the data at
 * runtime.
 *
 * The usage of the vtree_node is embedding it in your own type. The @offset is
 * used to locate vtree_node as a subtype. In otherwords, the @children should
 * be a vector of `vtree_node *`. The depth of the tree shouldn't be too
 * deep coz some methods are implemented recursively
 *
 * TODO: provide a make static method. To move all the children in the same vector
 */
struct vtree_node {
	//we can take the advantage of the sorting and search
	vector_t children;
	struct vtree_node *parent;
	off_t offset;
};

//the two helper function that can switch between base class and subclass
void *vtree_container(struct vtree_node *p); //you can also use container of

/**
 *
 * @offset is the integer offset that used to access vtree_node with a base adress
 */
void vtree_node_init(struct vtree_node *n, unsigned int offset);


static inline size_t
vtree_len(const struct vtree_node *n)
{
	return n->children.len;
}

static inline struct vtree_node *
vtree_ith_child(struct vtree_node *n, off_t i)
{
	return *(struct vtree_node **)vector_at(&n->children, i);
}

/**
 * @brief the function will find the original address of object from c, and copy from that
 *
 * return the newly added child
 */
static inline void
vtree_node_add_child(struct vtree_node* p, struct vtree_node *c)
{
	c->parent = p;
	struct vtree_node **e = vector_newelem(&p->children);
	*e = c;
}

static inline void
vtree_node_insert(struct vtree_node *p, struct vtree_node *c, off_t idx)
{
	vector_insert(&p->children, &c, idx);
	c->parent = p;
}

static inline void
vtree_node_remove(struct vtree_node *p, off_t i)
{
	vector_erase(&p->children, i);
}

/* shift vtree_node by step */
void vtree_node_shift(struct vtree_node *v, bool forward);

//the argument should be two vtree node, you should use container to convert it to your own type
void vtree_sort(struct vtree_node *p, int (*cmpfun)(const void *, const void *));

off_t vtree_find(struct vtree_node *p, const void *,
		 int (*cmpfun)(const void *, const struct vtree_node *));

void vtree_print(const struct vtree_node *p,
		 void (*print)(const struct vtree_node *), int indent);

//a DFS/BFS visiting algorithm
int vtree_iterate(const struct vtree_node *root, void *data,
		  void (*visit)(const struct vtree_node *, void *));

struct vtree_node *
vtree_search(const struct vtree_node *root, void *data,
	     int (*cmpfun)(const void *, const struct vtree_node *));


/* destroy the tree depth first */
void vtree_destroy(struct vtree_node *p, void (*freefun)(void *));

/* destroy all the children of the tree but leaves p alive. ONLY Use this if p
 * is not allocated on the heap */
void vtree_destroy_children(struct vtree_node *p, void (*freefun)(void *));



//maybe you need a search tree


#ifdef __cplusplus
}
#endif

#endif /*EOF */
