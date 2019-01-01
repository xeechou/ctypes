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


//the argument should be two vtree node, you should use container to convert it to your own type
void vtree_sort(struct vtree_node *p, int (*cmpfun)(const void *, const void *));
struct vtree_node *vtree_find(struct vtree_node *p, struct vtree_node *n,
			      int (*cmpfun)(const void *, const void *));
void vtree_print(const struct vtree_node *p,
		 void (*print)(const struct vtree_node *), int indent);

/* destroy the tree depth first */
void vtree_destroy(struct vtree_node *p, void (*freefun)(void *));



//maybe you need a search tree


#ifdef __cplusplus
}
#endif

#endif /*EOF */
