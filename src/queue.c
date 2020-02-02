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
#include "queue.h"

void
queue_init(queue_t *queue, size_t esize, void (*free_func)(void *))
{
	vector_t *vec = &queue->vec;
	vector_init(vec, esize, free_func);
	queue->head = 0;
}

//it shouldn't be bug anymore
void
queue_pop(queue_t *queue)
{
	vector_t *v = &queue->vec;
	typedef char elem_t[v->elemsize];
	//boundary check
	if (queue->head >= v->len)
		return;
	//shrink
	if (queue->head >= (3 * v->len / 4)) {
		size_t s = (v->len - queue->head) * v->elemsize;
		void *new_copy = malloc(s * 2);
		memmove(new_copy, (elem_t *)v->elems + queue->head, s);
		//the rest, use the free function to clean up
		if (v->free)
			for (int i = 0; i < queue->head; i++)
				v->free(vector_at(v, i));
		free(v->elems);
		v->elems = new_copy;
		v->len = s / v->elemsize;
		v->alloc_len = 2 * s / v->elemsize;
		queue->head = 0;
	}
	queue->head++;
}

void
queue_append(queue_t *queue, void *e)
{
	vector_append(&queue->vec, e);
}

void *
queue_top(queue_t *queue)
{
	return vector_at(&queue->vec, queue->head);
}

void
queue_destroy(queue_t *q)
{
	vector_destroy(&q->vec);
}

bool
queue_empty(queue_t *queue)
{
	return (queue->head >= queue->vec.len || queue->vec.len == 0) ? true : false;
}
