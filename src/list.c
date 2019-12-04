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
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void
list_init(list_t *list)
{
	list->prev = list;
	list->next = list;
}

//insert after the elem
void
list_insert(list_t *list, list_t *elm)
{
	elm->prev = list;
	elm->next = list->next;
	list->next->prev = elm;
	list->next = elm;
}


list_t *
list_insert_list(list_t *list, list_t *other)
{
	if (!other || other->next == other)
		return other;
	list_t *l_first = list->next;
	list_t *o_end = other->prev;
	list_t *o_first = other->next;

	o_first->prev = list;
	o_end->next = l_first;
	l_first->prev = o_end;
	list->next = o_first;
	//clean up
	other->next = other->prev = other;
	return other;
}

void
list_append(list_t *list, list_t *elm)
{
	elm->prev = list->prev;
	elm->next = list;
	list->prev->next = elm;
	list->prev = elm;
}

/* append @other to @list */
list_t *
list_append_list(list_t *list, list_t *other)
{
	//short-circuit
	if (!other || other->next == other)
		return other;

	list_t *o_end = other->prev;
	list_t *o_first = other->next;
	list_t *l_end = list->prev;

	list->prev = o_end;
	o_end->next = list;
	o_first->prev = l_end;
	l_end->next = o_first;
	//clean up
	other->next = other->prev = other;
	return other;
}

//swap should be safe if one and another are both valid
void
list_swap(list_t *one, list_t *another)
{
	list_t tmp = *one;
	//update one,
	one->next = another->next;
	one->prev = another->prev;
	//update one->prev,
	one->prev->next = another;
	//update one->next,
	one->next->prev = another;
	//update another,
	another->next = tmp.next;
	another->prev = tmp.prev;
	//update another->prev,
	another->prev->next = one;
	//update another->next,
	another->next->prev = one;
}

/* Insert a new header to the list_t @header */
//static inline void
//list_insert_header(list_t **header, list_t *elem)
//{
//	list_append_elem(header, elem);
//	*header = elem;
//}
/*
static inline void
list_remove_update(list_t **header, list_t *elem)
{
	//remove one node from the list_t and update the header if necessary,
	//there could be following boundary situations:
	//0) *elem == header, but not only elem
	//1) *elem is the only elem in the list
	list_t *prev = elem->prev;
	list_t *next = elem->next;
	prev->next = next;
	next->prev = prev;
	//case 0: elem == header
	if (elem == *header)
		*header = next;
	//case 1: the last elem
	//there is no assignment to elem directly, so elem->next shouldn't change
	if (elem->next == elem)
		*header = NULL;
}
*/

void
list_remove(list_t *elm)
{
	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = NULL;
	elm->prev = NULL;
}

int
list_length(const list_t *list)
{
	struct list_t *e;
	int count;

	count = 0;
	e = list->next;
	while (e != list) {
		e = e->next;
		count++;
	}

	return count;
}

bool
list_empty(const list_t *header)
{
	return header->next == header;
}



/* int main(int argc, char *argv[]) */
/* { */
/*	struct int_list { */
/*		list_t node; */
/*		int elem; */
/*	}; */

/*	struct int_list head; */
/*	struct int_list head1; */
/*	struct int_list a = { */
/*		.elem = 1 */
/*	}; */
/*	struct int_list b = { */
/*		.elem = 2 */
/*	}; */
/*	struct int_list c = { */
/*		.elem = 3 */
/*	}; */
/*	struct int_list d = { */
/*		.elem = 4 */
/*	}; */
/*	list_init(&head.node); */
/*	list_init(&head1.node); */
/*	printf("head0 is empty? %d\n", list_empty(&head.node)); */
/*	printf("head1 length? %d\n", list_length(&head1.node)); */
/*	list_insert(&head.node, &a.node); //working */
/*	list_insert(&head.node, &b.node); */
/*	list_append(&head1.node, &c.node); //working */
/*	list_append(&head1.node, &d.node); */
/*	struct int_list *itr; */
/*	list_for_each(itr, &head.node, node) { */
/*		printf("%d\t", itr->elem); */
/*	} */
/*	printf("\n"); */
/*	list_for_each(itr, &head1.node, node) { */
/*		printf("%d\t", itr->elem); */
/*	} */
/*	printf("\n"); */
/*	list_insert_list(&head.node, &head1.node); */
/*	list_for_each(itr, &head.node, node) { */
/*		printf("%d\t", itr->elem); */
/*	} */
/*	printf("\n"); */

/*	return 0; */
/* } */
