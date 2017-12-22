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
};

#define vtree_ptr_size sizeof(struct vtree_node *)

#ifdef __cpluscplus
}
#endif

#endif
