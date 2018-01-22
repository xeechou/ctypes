#include <stdbool.h>
#include <string.h>
#include "hash.h"

static uint64_t next_prime(uint64_t n)
{
	//make it odd
	if (n < 3)
		return 11;
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

/**
 * cet function guarantee de retourner qqchs
 *
 * NULL: not found in all the entries.
 * data: if hash_empty or eq. Then you need to compare it yourself
 */
static void*
_dhash_search(dhashtab_t *t, const void *elem)
{
	uint64_t hv0 = t->hash0(elem);
	uint64_t hv1 = t->hash1(elem);
	for (int i = 0; i < t->data.len; i++) {
		uint64_t idx = (hv0 + i * hv1) % t->data.alloc_len;
		int c = t->cmp(elem, cvector_at(&t->data, idx));
		switch(c) {
		case hash_empty:
			return vector_at(&t->data, idx);
			break;
		case hash_eq:
			return vector_at(&t->data, idx);
			break;
		case hash_neq:
			continue;
		}
	}
	return NULL;
}

/**
 * NULL: not found,
 * data: found
 */
const void *
dhash_search(dhashtab_t *t, const void *elem)
{
	void *addr = _dhash_search(t, elem);
	if (!addr || t->cmp(elem, addr) == hash_empty)
		return NULL;
	else
		return addr;
}

void
dhash_insert(dhashtab_t *t, const void *elem)
{
	expand_dhasht_if_needed(t);
	void *addr = _dhash_search(t, elem);
	hash_cmp_val r = t->cmp(elem, addr);
	if (r == hash_empty) {
		memcpy(addr, elem, t->data.elemsize);
		t->data.len += 1;
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
