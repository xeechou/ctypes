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

#ifndef CTYPES_LIST_H
#define CTYPES_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "helpers.h"

#ifdef __cplusplus
#extern "C" {
#endif

/**
 * @brief double-link list
 * I guess this one you have one head extra which is just head
 */
typedef struct list_t {
	struct list_t *prev;
	struct list_t *next;
} list_t;


/** use this one if don't need to change the link structure
 *
 */
#define list_for_each(pos, head, member)				\
	for (pos = container_of_sm((head)->next, pos, member);	\
	     &pos->member != (head);					\
	     pos = container_of_sm(pos->member.next, pos, member))

/** use this one if you are in destructors
 *
 */
#define list_for_each_safe(pos, tmp, head, member)			\
	for (pos = container_of_sm((head)->next, pos, member),		\
	     tmp = container_of_sm((pos)->member.next, tmp, member);	\
	     &pos->member != (head);					\
	     pos = tmp,							\
	     tmp = container_of_sm(pos->member.next, tmp, member))

/**
 * @brief make a list
 */
void list_init(list_t *list);

/**
 * @brief insert a list int beginning
 */
void list_insert(list_t *list, list_t *elm);

list_t* list_insert_list(list_t *list, list_t *another);

/**
 * @brief append @other to @list at the end
 */
void
list_append(list_t *list, list_t *other);

/**
 * @brief append a list @other to the @list
 *
 * So the head of the other is lost, we return the head, you can decide to free
 * it or not
 */
list_t *list_append_list(list_t *list, list_t *ol);

/**
 *
 * swap @one and @another
 */
void list_swap(list_t *one, list_t *another);

int list_length(const list_t *list);

void list_remove(list_t *elm);

bool list_empty(const list_t *header);

//void
//list_swap_header_update(list_t **header, list_t *another);


#ifdef __cplusplus
}
#endif

#endif /* EOF */
