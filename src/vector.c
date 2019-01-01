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
	if (f)
		v->free = f;
	else
		v->free = dummy_free;
	v->elemsize = esize;
	v->len = 0;
	v->alloc_len = 4;
	return true;
}

void
vector_init_zero(vector_t *v, size_t esize, freefun f)
{
	v->elems = NULL;
	v->free = f;
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
		v->elems = realloc(v->elems, v->len);
		v->alloc_len = v->len;
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
	if (idx >= v->len)
		return NULL;
	return (unsigned char *)v->elems + v->elemsize * idx;
}


const void *
cvector_at(const vector_t *v, size_t idx)
{
	if (idx >= v->len)
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
	v->len--;
	shrink_vector_if_need(v);
}

void vector_resize(vector_t *v, size_t n)
{
	v->elems = realloc(v->elems, v->elemsize * n);
	v->alloc_len = n;
	v->len = n;
}

/*
int main(int argc, char *argv[argc])
{
	struct a {
		char t[9];
	};
	struct a bb;
	strcpy((char *)&(bb.t), "aaaaaaaa");

	vector_t v;
	vector_init(&v, sizeof(struct a), NULL);
	int i;
	for (i = 0; i < 100; i++)
		vector_append(&v, &bb);
	for (i = 0; i < 100; i++)
		vector_pop(&v);
	return 0;
}
*/
