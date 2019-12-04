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

#include <stdbool.h>
#include <string.h>
#include "hash.h"

static uint64_t next_prime(uint64_t n)
{
	//make it odd
	if (n % 2 == 0)
		n += 1;
	for (uint64_t c = n*2+1; true; c++) {
		bool found = true;
		for (uint64_t x = 2; x < c/2; x++) {
			if (c % x == 0) {
				found = false;
				break;
			}
		}
		if (found)
			return c;
	}
}


static void
expand_dhasht_if_needed(dhashtab_t *table)
{
	if (table->indices.elemsize == 0)
		vector_init(&table->indices, sizeof(int), NULL);
	if (table->indices.len >= table->data.alloc_len / 2) {
		vector_t backup;
		vector_t backup_idx;
		memcpy(&backup, &table->data, sizeof(vector_t));
		memcpy(&backup_idx, &table->indices, sizeof(vector_t));
		//dont free the elements here
		backup.free = dummy_free;
		vector_init(&table->indices, sizeof(int), NULL);

		table->data.alloc_len = next_prime(table->data.alloc_len);
		table->data.len = table->data.alloc_len;
		table->data.elems = xcalloc(table->data.alloc_len, table->data.elemsize);
		for (int i = 0; i < backup_idx.len; i++) {
			int idx = *(int *)cvector_at(&backup_idx, i);
			dhash_insert(table, cvector_at(&backup, idx));
		}
		vector_destroy(&backup);
		vector_destroy(&backup_idx);
	}
}

/**
 * cet function guarantee de retourner qqchs
 *
 * -1: not found in all the entries.
 * data: if hash_empty or eq. Then you need to compare it yourself
 */
static int
_dhash_search(dhashtab_t *t, const void *elem)
{
	uint64_t hv0 = t->hash0(elem);
	uint64_t hv1 = t->hash1(elem);
	for (int i = 0; i < t->data.alloc_len; i++) {
		int idx = (hv0 + i * hv1) % t->data.alloc_len;
		int c = t->cmp(elem, cvector_at(&t->data, idx));
		switch(c) {
		case hash_empty:
			return idx;
			break;
		case hash_eq:
			return idx;
			break;
		case hash_neq:
			continue;
		}
	}
	return -1;
}

/**
 * NULL: not found,
 * data: found
 */
const void *
dhash_search(dhashtab_t *t, const void *elem)
{
	int idx = _dhash_search(t, elem);
	if (idx == -1 || t->cmp(elem, cvector_at(&t->data, idx)) == hash_empty)
		return NULL;
	else
		return cvector_at(&t->data, idx);
}

void
dhash_insert(dhashtab_t *t, const void *elem)
{
	expand_dhasht_if_needed(t);
	int idx = _dhash_search(t, elem);
	void *addr = vector_at(&t->data, idx);
	hash_cmp_val r = t->cmp(elem, addr);
	if (r == hash_empty) {
		memcpy(addr, elem, t->data.elemsize);
		*(int *)vector_newelem(&t->indices) = idx;
	}
}


void dhash_init(dhashtab_t *t, hash_func_t h0, hash_func_t h1, hash_cmp_func_t cmp,
		size_t esize, freefun free)
{
	t->data.elemsize = esize;
	t->data.alloc_len = 11;
	t->data.elems = xcalloc(t->data.alloc_len, esize);
	t->data.len = 11;
	t->data.free = (free) ? free : dummy_free;

	vector_init(&t->indices, sizeof(int), NULL);

	t->hash0 = h0;
	t->hash1 = h1;
	t->cmp = cmp;
}

void dhash_destroy(dhashtab_t *t)
{
	for (int i = 0; i < t->indices.len; i++) {
		void *p = vector_at(&t->data, *(int *)cvector_at(&t->indices, i));
		t->data.free(p);
	}
	xfree(t->data.elems);
	vector_destroy(&t->indices);
}



/////////////////////////////////////////////////////////////////
///////////////// commonly used hash function ///////////////////
/////////////////////////////////////////////////////////////////
uint64_t hash_djb2(const void *d)
{
	const char *str = *(const char **)d;
	uint64_t hash = 5381;
	int c;
	while ((c = *str++) != '\0') {
		hash = ((hash << 5) + hash + c);
	}
	return hash;
}

uint64_t hash_sdbm(const void *d)
{
	const char *str = *(const char **)d;
	uint64_t hash = 0;
	int c;
	while ((c = *str++) != '\0')
		hash = c + (hash << 6) + (hash << 16) - hash;
	return hash;
}

hash_cmp_val hash_cmp_str(const void *a, const void *intable)
{
	const char *stra = *(const char **)a;
	const char *strb = *(const char **)intable;
	if (!strb)
		return hash_empty;
	else
		return  (strcmp(stra, strb) == 0) ? hash_eq : hash_neq;
}
