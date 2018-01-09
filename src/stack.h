#ifndef CTYPES_STACK_H
#define CTYPES_STACK_H


#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	vector_t vec;
	off_t head;
} stack_t;

/**
 * @brief implement the stack by vector
 */
void stack_init(stack_t *stack, size_t esize, void (*free_func)(void *));


/**
 * pop the head off
 */
void stack_pop(stack_t *stack);

void stack_append(stack_t *stack, void *e);

void *stack_top(stack_t *stack);

const bool stack_empty(stack_t *stack);



#ifdef __cplusplus
}
#endif


#endif /* EOF */
