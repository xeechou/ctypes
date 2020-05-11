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
#include <ctypes/hash.h>

static uint64_t
next_prime(uint64_t n)
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
	typedef unsigned char key_t[table->keys.elemsize];
	key_t null_key;

	memset(&null_key, 0, sizeof(key_t));
	if ((int)table->allocated >= table->data.len / 2) {
		vector_t backup_keys = table->keys;
		vector_t backup_elems = table->data;
		int new_size = next_prime(backup_keys.len);

		vector_init_zero(&table->keys, table->keys.elemsize,
		                 table->keys.free);
		vector_init_zero(&table->data, table->data.elemsize,
		                 table->data.free);
		vector_reserve(&table->keys, new_size);
		vector_reserve(&table->data, new_size);
		table->keys.len = new_size;
		table->data.len = new_size;

		//have to make sure keys filled with zero
		memset(table->keys.elems, 0,
		       table->keys.len * table->keys.elemsize);

		for (int i = 0; i < backup_keys.len; i++) {
			key_t *key =  vector_at(&backup_keys, i);
			void *data = vector_at(&backup_elems, i);
			if (memcmp(key, &null_key, sizeof(key_t)))
				dhash_insert(table, key, data);
		}
		backup_keys.free = dummy_free;
		backup_elems.free = dummy_free;
		vector_destroy(&backup_keys);
		vector_destroy(&backup_elems);
	}
}

/**
 * cet function guarantee de retourner qqchs
 *
 * -1: not found in all the entries.
 * data: if hash_empty or eq. Then you need to compare it yourself
 */
static int
_dhash_search(dhashtab_t *t, const void *key)
{
	uint64_t hv0 = t->hash0(key);
	uint64_t hv1 = t->hash1(key);
	for (int i = 0; i < t->keys.len; i++) {
		int idx = (hv0 + i * hv1) % t->data.len;
		int c = t->cmp(key, cvector_at(&t->keys, idx));
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
void *
dhash_search(dhashtab_t *t, const void *key)
{
	int idx = _dhash_search(t, key);
	if (idx == -1 || t->cmp(key, cvector_at(&t->keys, idx)) == hash_empty)
		return NULL;
	else
		return vector_at(&t->data, idx);
}

void
dhash_insert(dhashtab_t *t, const void *key, const void *elem)
{
	expand_dhasht_if_needed(t);
	int idx = _dhash_search(t, key);
	void *keyaddr = vector_at(&t->keys, idx);
	void *addr = vector_at(&t->data, idx);
	hash_cmp_val r = t->cmp(key, keyaddr);
	if (r == hash_empty) {
		memcpy(keyaddr, key, t->keys.elemsize);
		memcpy(addr, elem, t->data.elemsize);
		t->allocated += 1;
	}
}


void dhash_init(dhashtab_t *t, hash_func_t h0, hash_func_t h1, hash_cmp_func_t cmp,
                size_t keysize, size_t esize, freefun keyfree, freefun datafree)
{
	vector_init_zero(&t->keys, keysize, keyfree);
	vector_init_zero(&t->data, esize, datafree);
	vector_reserve(&t->data, 11);
	vector_reserve(&t->keys, 11);
	t->data.len = 11;
	t->keys.len = 11;
	memset(t->keys.elems, 0,
	       t->keys.len * t->keys.elemsize);
	t->allocated = 0;

	t->hash0 = h0;
	t->hash1 = h1;
	t->cmp = cmp;
}

void dhash_destroy(dhashtab_t *t)
{
	if (t->keys.free != dummy_free || t->data.free != dummy_free) {
		for (int i = 0; i < t->keys.len; i++) {
			void *key = vector_at(&t->keys, i);
			void *dat = vector_at(&t->data, i);

			if (key) {
				t->keys.free(key);
				t->data.free(dat);
			}
		}
	}
	free(t->keys.elems);
	free(t->data.elems);
	vector_init_zero(&t->keys, t->keys.elemsize, t->keys.free);
	vector_init_zero(&t->data, t->data.elemsize, t->data.free);
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
