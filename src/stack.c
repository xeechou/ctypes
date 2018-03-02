#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "vector.h"
#include "stack.h"

void
cstack_init(cstack_t *cstack, size_t esize, void (*free_func)(void *))
{
	vector_t *vec = &cstack->vec;
	vector_init(vec, esize, free_func);
}

void
cstack_pop(cstack_t *cstack)
{
	vector_pop(&cstack->vec);
}

void
cstack_append(cstack_t *cstack, void *e)
{
	vector_append(&cstack->vec, e);
}

void *
ccstack_top(cstack_t *cstack)
{
	return vector_at(&cstack->vec, cstack->vec.len-1);
}

const bool
cstack_empty(cstack_t *cstack)
{
	return cstack->vec.len <= 0;
}

void
cstack_destroy(cstack_t *s)
{
	vector_destroy(&s->vec);
}
