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
