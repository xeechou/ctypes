#ifndef CTYPES_HASH_H
#define CTYPES_HASH_H

#include <stdint.h>
#include <stdbool.h>
#include <search.h>

#include "vector.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	hash_empty, hash_eq, hash_neq,
} hash_cmp_val;

//the b is the one in the hash table
typedef hash_cmp_val (*hash_cmp_func_t)(const void *a, const void *intable);
typedef uint64_t (*hash_func_t)(const void *addr);

/// common hash functions and compare functions
hash_cmp_val hash_cmp_str(const void *sa, const void *sb);
uint64_t hash_djb2(const void *str);
uint64_t hash_sdbm(const void *str);




//in order to get the double hashing to work, the table size should co-prime to
//the output of second hash function, so the easiest way is just make table size primal

// we need to implement a double hash data type.
typedef struct dhash_table {
	//the size of the hash table need to be a prime number
	vector_t data;
	hash_func_t hash0;
	hash_func_t hash1;
	hash_cmp_func_t cmp;
} dhashtab_t;


void dhash_init(dhashtab_t *t, hash_func_t h0, hash_func_t h1, hash_cmp_func_t cmp,
		size_t esize, freefun free);

void dhash_insert(dhashtab_t *t, void *elem);

void dhash_destroy(dhashtab_t *t);



#ifdef __cplusplus
}
#endif


#endif
