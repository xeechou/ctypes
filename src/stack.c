#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "vector.h"
#include "stack.h"

void
stack_init(stack_t *stack, size_t esize, void (*free_func)(void *))
{
	vector_t *vec = &stack->vec;
	vector_init(vec, esize, free_func);
}

//allor, c'est facile
void
stack_pop(stack_t *stack)
{
	vector_t *v = &stack->vec;
	typedef char elem_t[v->elemsize];
	//boundary check
	if (stack->head >v->len)
		return;
	//shrink
	if (stack->head >= (3 * v->len / 4)) {
		size_t s = (v->len - stack->head) * v->elemsize;
		void *new_copy = malloc(s);
		memmove(new_copy, (elem_t *)v->elems + stack->head, s);
		//the rest, use the free function to clean up
		for (int i = 0; i < stack->head; i++)
			v->free(vector_at(v, i));
		free(v->elems);
		v->elems = new_copy;
		v->len = s;
		v->alloc_len = s;
	}
	stack->head++;
}

void
stack_append(stack_t *stack, void *e)
{
	vector_append(&stack->vec, e);
}

void *
stack_top(stack_t *stack)
{
	return vector_at(&stack->vec, stack->head);
}

const bool
stack_empty(stack_t *stack)
{
	return (stack->head >= stack->vec.len || stack->vec.len == 0) ? true : false;
}
