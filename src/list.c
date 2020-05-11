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
#include <ctypes/list.h>

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
