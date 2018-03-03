#ifndef CTYPES_STACK_H
#define CTYPES_STACK_H

//this is not stack, this is a queue
#include <pthread.h>
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

const bool cstack_empty(cstack_t *stack);

void cstack_destroy(cstack_t *s);





#ifdef __cplusplus
}
#endif


#endif /* EOF */
