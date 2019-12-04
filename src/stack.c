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
cstack_top(cstack_t *cstack)
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
