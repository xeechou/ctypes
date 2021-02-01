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

#ifndef CTYPES_VECTOR_H
#define CTYPES_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include "helpers.h"

typedef struct {
	void *elems;
	size_t elemsize;
	int len;
	int alloc_len;
	void (*free) (void *);
} vector_t;

size_t vector_memsize(const vector_t *v);

bool vector_init(vector_t *v, size_t esize, freefun f);
void vector_init_zero(vector_t *v, size_t esize, freefun f);

void vector_destroy(vector_t *v);
/**
 * @brief return a new element address for you to copy, since the copy function
 * is usually not efficient for small data type,
 * @return (type *) where type is the type of the elem
 */
void *vector_newelem(vector_t *v);

/**
 * @brief iterator over an array
 *
 * taken from wl_array, provides a easier way to iterator through a vector
 */
#define vector_for_each(pos, v)			\
	for (pos = (v)->elems;						\
	     (const char *) pos < ((const char *) (v)->elems + (v)->len * (v)->elemsize);\
	     pos = (typeof(pos))((const char *)pos + (v)->elemsize))

/**
 * @brief deep copy a vector
 */
void vector_copy(vector_t *dst, vector_t *src);

/**
 * @brief deep copy with copy constructor
 */
void vector_copy_complex(vector_t *dst, vector_t *src,
			 void(*assign)(void *, const void *));

/**
 * @brief ability to random access the vector
 * @return (type *) of the type you inserted
 */
void *vector_at(vector_t *v, size_t idx);

/**
 * @brief ability of constant random access
 */
const void *
cvector_at(const vector_t *v, size_t idx);

/**
 * @brief for big datatype, you can use memcopy method to copy element
 */
void *
vector_append(vector_t *v, void *e);

/**
 * @brief remove one and the end
 */
void
vector_pop(vector_t *v);

/**
 * @brief insert at specific location
 */
void vector_insert(vector_t *v, void *e, off_t idx);

void vector_erase(vector_t *v, off_t idx);

/**
 * @brief trunk or expand the size of the vector
 */
void vector_resize(vector_t *v, size_t n);

/**
 * @brief expand the allocation of the vector
 */
void vector_reserve(vector_t *v, size_t n);


#ifdef __cplusplus
}
#endif

#endif /* EOF */
