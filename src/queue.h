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

#ifndef CTYPE_QUEUE_H
#define CTYPE_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vector.h"

typedef struct {
	vector_t vec;
	off_t head;
} queue_t;

/**
 * @brief implement the queue using vector
 */
void queue_init(queue_t *queue, size_t esize, void (*free_func)(void *));
//hope that I don't need implement a dequeue later

/**
 * pop the head off
 */
void queue_pop(queue_t *queue);

void queue_append(queue_t *queue, void *e);

void *queue_top(queue_t *queue);

const bool queue_empty(queue_t *queue);

void queue_destroy(queue_t *q);


#ifdef __cplusplus
}
#endif

#endif /* EOF */
