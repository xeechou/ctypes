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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "helpers.h"
#include "vector.h"

/* A C vector is just a growing array, since we can index it,
 */
void dummy_free(void *addr) {}


bool
vector_init(vector_t *v, size_t esize, freefun f)
{
	v->elems = (void *) malloc(esize * 4);
	if (!v->elems)
		return false;
	v->free = f ? f : dummy_free;
	v->elemsize = esize;
	v->len = 0;
	v->alloc_len = 4;
	return true;
}

void
vector_init_zero(vector_t *v, size_t esize, freefun f)
{
	v->elems = NULL;
	v->free = f ? f : dummy_free;
	v->elemsize = esize;
	v->len = 0;
	v->alloc_len = 0;
}


size_t
vector_memsize(const vector_t *v)
{
	return v->elemsize * v->alloc_len;
}

void
vector_destroy(vector_t *v)
{
	typedef char elem_t[v->elemsize];
	elem_t *p = (elem_t *)v->elems;
	for (int i = 0; i < v->len; i++)
		v->free(p++);
	free(v->elems);
	vector_init_zero(v, v->elemsize, NULL);
}

//we can actually make a general expand function
static bool
expand_vector_if_need(vector_t *v)
{
	//avoid v->alloc = 0 problem
	size_t new_alloc = (v->alloc_len == 0) ?  4 : v->alloc_len * 2;
	if (v->len >= v->alloc_len) {
#ifdef DEBUG_TYPE
		fprintf(stderr, "vector expanding at length %d\n", v->len);
#endif
		v->alloc_len = new_alloc;
		void *tmp = v->elems;
		v->elems = realloc(v->elems, v->elemsize * v->alloc_len);
		if (!v->elems) {
			fprintf(stderr, "vector alloc failed!\n");
			v->elems = tmp;
			return false;
		}

	}
	return true;
}

static bool
shrink_vector_if_need(vector_t *v)
{
	if (v->len <= v->alloc_len / 4) {
#ifdef DEBUG_TYPE
		fprintf(stderr, "vector shrinks at length %d\n", v->len);
#endif
		v->elems = realloc(v->elems, v->len * 2 * v->elemsize);
		v->alloc_len = v->len * 2;
	}
	return true;
}


/**
 * return a new element address for you to copy, since the copy function is
 * usually not efficient
 */
void *
vector_newelem(vector_t *v)
{
	typedef char elem_t[v->elemsize];
	expand_vector_if_need(v);
	void *elem = (elem_t*)v->elems + v->len;
	v->len += 1;
	return elem;
}

void *
vector_at(vector_t *v, size_t idx)
{
	if (idx >= (unsigned)v->len)
		return NULL;
	return (unsigned char *)v->elems + v->elemsize * idx;
}


const void *
cvector_at(const vector_t *v, size_t idx)
{
	if (idx >= (unsigned)v->len)
		return NULL;
	return (unsigned char *)v->elems + v->elemsize * idx;
}


void *
vector_append(vector_t *v, void *e)
{
	typedef char elem_t[v->elemsize];
	expand_vector_if_need(v);
	void *elem = (elem_t*)v->elems + v->len;
	memcpy(elem, e, v->elemsize);
	v->len += 1;
	return elem;
}


void
vector_pop(vector_t *v)
{
	//avoid memory leak
	void *addr = vector_at(v, v->len-1);
	if (v->free)
		v->free(addr);
	//shrink
	v->len--;
	shrink_vector_if_need(v);
}

void
vector_resize(vector_t *v, size_t n)
{
	size_t new_size;

	if ((size_t)v->alloc_len >= n || n == 0) {
		v->len = n;
		return;
	}

	new_size = (v->len) == 0 ? 1 : v->len;
	while (new_size < n)
		new_size *= 2;

	v->elems = realloc(v->elems, v->elemsize * new_size);
	v->alloc_len = new_size;
	v->len = n;
}

void
vector_reserve(vector_t *v, size_t n)
{
	void *data;
	if (n <= (unsigned)v->alloc_len)
		return;

	data = realloc(v->elems, v->elemsize * n);
	if (data) {
		v->elems = data;
		v->alloc_len = n;
	}
}

void
vector_copy(vector_t *dst, vector_t *src)
{
	dst->elemsize = src->elemsize;
	dst->free = src->free;
	vector_resize(dst, src->len);
	memcpy(dst->elems, src->elems, src->elemsize * src->len);
}

void
vector_copy_complex(vector_t *dst, vector_t *src,
		    void(*assign)(void *, const void *))
{
	dst->elemsize = src->elemsize;
	dst->free = src->free;
	vector_resize(dst, src->len);
	memset(dst->elems, 0, dst->len * dst->elemsize);
	for (int i = 0; i < dst->len; i++)
		assign(vector_at(dst, i), vector_at(src, i));
}

void
vector_insert(vector_t *v, void *e, off_t idx)
{
	typedef char elem_t[v->elemsize];

	if (idx >= v->len)
		vector_append(v, e);
	else {
		int new_size = (v->len+1) >= v->alloc_len ?
			2 * v->alloc_len : v->alloc_len;
		void *new_elems = malloc(new_size * v->elemsize);

		memcpy(new_elems, v->elems, (idx * v->elemsize));
		memcpy((elem_t *)new_elems + idx,
		       e, v->elemsize);
		memcpy((elem_t *)new_elems + idx+1,
			(elem_t *)v->elems + idx,
			(v->len - idx) * v->elemsize);

		free(v->elems);
		v->len += 1;
		v->alloc_len = new_size;
		v->elems = new_elems;
	}
}

void
vector_erase(vector_t *v, off_t idx)
{
	typedef char elem_t[v->elemsize];

	if (idx >= v->len)
		vector_pop(v);
	else {
		int new_size = (v->len-1) <= v->alloc_len * 0.25 ?
			v->alloc_len * 0.5 : v->alloc_len;

		//you cannot do pop it will kill the last elem
		void *new_elems = malloc(v->elemsize * new_size);
		memcpy(new_elems, v->elems, (idx * v->elemsize));
		memcpy((elem_t *)new_elems + idx,
		       (elem_t *)v->elems + idx+1,
		       (v->len-idx-1) * v->elemsize);

		void *to_erase = vector_at(v, idx);
		if (v->free)
			v->free(to_erase);
		free(v->elems);

		v->elems = new_elems;
		v->len -= 1;
		v->alloc_len = new_size;
	}
}
