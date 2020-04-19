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

#ifndef CTYPES_HASH_H
#define CTYPES_HASH_H

#include <stdint.h>
#include <stdbool.h>
#include <search.h>

#include "helpers.h"
#include "vector.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	hash_empty, hash_eq, hash_neq,
} hash_cmp_val;

typedef hash_cmp_val (*hash_cmp_func_t)(const void *a, const void *intable);
typedef uint64_t (*hash_func_t)(const void *addr);

/* hash routines for strings */
hash_cmp_val
hash_cmp_str(const void *sa, const void *sb);

uint64_t
hash_djb2(const void *str);

uint64_t
hash_sdbm(const void *str);




//in order to get the double hashing to work, the table size should co-prime to
//the output of second hash function, so the easiest way is just make table size primal

// we need to implement a double hash data type.
typedef struct dhash_table {
	//the size of the hash table need to be a prime number
	vector_t keys;
	vector_t data;
	size_t allocated;
	hash_func_t hash0;
	hash_func_t hash1;
	hash_cmp_func_t cmp;
} dhashtab_t;


void
dhash_init(dhashtab_t *t, hash_func_t h0, hash_func_t h1, hash_cmp_func_t cmp,
           size_t keysize, size_t esize, freefun keyfree, freefun datafree);
void
dhash_destroy(dhashtab_t *t);

void
dhash_insert(dhashtab_t *t, const void *key, const void *elem);

void
dhash_destroy(dhashtab_t *t);

const void *
dhash_search(dhashtab_t *t, const void *key);


#ifdef __cplusplus
}
#endif


#endif
