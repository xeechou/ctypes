#ifndef CTYPES_HASH_H
#define CTYPES_HASH_H

#include <stdint.h>
#include <stdbool.h>
#include <search.h>
//common hash function routines
//the implementation by djb2
uint64_t hash_string(const char *str)
{
	uint64_t hash = 5381;
	int c;
	while ((c = *str++) != '\0') {
		hash = ((hash << 5) + hash + c);
	}
	return hash;
}
//well, I mean you can use gnu hash tables

#endif
