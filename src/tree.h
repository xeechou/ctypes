#ifndef CTYPES_TREE_H
#define CTYPES_TREE_H

#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vtree_node {
	//we can take the advantage of the sorting and search
	vector_t children;
	struct vtree_node *parent;
	off_t offset;
};

//the two helper function that can switch between base class and subclass
struct vtree_node *vtree_cast(void *potential, const struct vtree_node *example);
void *vtree_container(struct vtree_node *p); //you can also use container of

/**
 * @tnodesize indicate the size of the object that contains vtree_node
 * @offset is the integer offset that used to access vtree_node with a base adress
 */
void vtree_node_init(struct vtree_node *n, unsigned int tnodesize, unsigned int offset);

/**
 * @brief the function will find the original address of object from c, and copy from that
 */
void vtree_node_add_child(struct vtree_node* p, struct vtree_node *c);

//sort and find uses the same comp function
void vtree_sort(struct vtree_node *p, int (*cmpfun)(const void *, const void *));
struct vtree_node *vtree_find(struct vtree_node *p, void *key,
			      int (*cmpfun)(const void *, const void *));
void vtree_print(const struct vtree_node *p,
		 void (*print)(const void *), int indent);

/* destroy the tree depth first */
void vtree_destroy(struct vtree_node *p, void (*freefun)(void *));



#ifdef __cplusplus
}
#endif

#endif /*EOF */
