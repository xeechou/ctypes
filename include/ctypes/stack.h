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

#ifndef CTYPES_STACK_H
#define CTYPES_STACK_H

//this is not stack, this is a queue
#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

//this name is conflict with gnu bits library
typedef struct {
	vector_t vec;
} cstack_t;

/**
 * @brief implement the stack by vector
 */
void cstack_init(cstack_t *stack, size_t esize, void (*free_func)(void *));


/**
 * pop the head off
 */
void cstack_pop(cstack_t *stack);

void cstack_append(cstack_t *stack, void *e);

void *cstack_top(cstack_t *stack);

bool cstack_empty(cstack_t *stack);

void cstack_destroy(cstack_t *s);





#ifdef __cplusplus
}
#endif


#endif /* EOF */
