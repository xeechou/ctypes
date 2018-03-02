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

const bool
queue_empty(queue_t *queue)
{
	return (queue->head >= queue->vec.len || queue->vec.len == 0) ? true : false;
}
