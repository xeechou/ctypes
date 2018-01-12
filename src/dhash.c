#include <stdbool.h>
#include <string.h>
#include "hash.h"

static uint64_t next_prime(uint64_t n)
{
	//make it odd
	if (n % 2 == 0)
		n += 1;
	for (uint64_t c = n+2; true; c++) {
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
	if (table->data.len >= table->data.alloc_len / 2) {
		table->data.alloc_len = next_prime(table->data.alloc_len);
		table->data.elems = realloc(table->data.elems, table->data.alloc_len);
	}
}


void
dhash_insert(dhashtab_t *t, void *elem)
{
	expand_dhasht_if_needed(t);
	uint64_t hv0 = t->hash0(elem);
	uint64_t hv1 = t->hash1(elem);
	for (int i = 0; i < t->data.len; i++) {
		uint64_t idx = (hv0 + i * hv1) % t->data.alloc_len;
		int c = t->cmp(elem, cvector_at(&t->data, idx));
		switch(c) {
		case hash_empty:
			memcpy(vector_at(&t->data, idx), elem, t->data.elemsize);
			t->data.len += 1;
			break;
		case hash_eq:
			break;
		case hash_neq:
			continue;
		}
	}
}

void dhash_init(dhashtab_t *t, hash_func_t h0, hash_func_t h1, hash_cmp_func_t cmp,
		size_t esize, freefun free)
{
	t->data.elemsize = esize;
	t->data.alloc_len = 11;
	t->data.elems = xcalloc(t->data.alloc_len, esize);
	t->data.len = 0;
	t->data.free = (free) ? free : dummy_free;

	t->hash0 = h0;
	t->hash1 = h1;
	t->cmp = cmp;
}

void dhash_destroy(dhashtab_t *t)
{
	vector_destroy(&t->data);
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
	if (*strb == '\0')
		return hash_empty;
	else
		return  (strcmp(stra, strb) == 0) ? hash_eq : hash_neq;
}
