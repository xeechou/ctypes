#ifndef CTYPES_VECTOR_H
#define CTYPES_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include "helpers.h"

typedef struct {
	void *elems;
	size_t elemsize;
	int len;
	int alloc_len;
	void (*free) (void *);
} vector_t;


bool
vector_init(vector_t *v, size_t esize, freefun f);
void
vector_destroy(vector_t *v);

/**
 * @brief return a new element address for you to copy, since the copy function
 * is usually not efficient for small data type,
 * @return (type *) where type is the type of the elem
 */
void *
vector_newelem(vector_t *v);

/**
 * @brief ability to random access the vector
 * @return (type *) of the type you inserted
 */
void *
vector_at(vector_t *v, size_t idx);

/**
 * @brief for big datatype, you can use memcopy method to copy element
 */
void
vector_append(vector_t *v, void *e);

/**
 * @brief remove one and the end
 */
void
vector_pop(vector_t *v);


#ifdef __cplusplus
}
#endif

#endif /* EOF */
