#ifndef CTYPES_LIST_H
#define CTYPES_LIST_H

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

#ifndef container_of_sm
#define container_of_sm(ptr, sample, member)				\
	(__typeof__(sample))((char *)(ptr) -				\
			     offsetof(__typeof__(*sample), member))
#endif


#define list_for_each(pos, head, member)				\
	for (pos = container_of_sm((head)->next, pos, member);	\
	     &pos->member != (head);					\
	     pos = tw_container_of(pos->member.next, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	for (pos = container_of_sm((head)->next, pos, member),		\
	     tmp = container_of_sm((pos)->member.next, tmp, member);	\
	     &pos->member != (head);					\
	     pos = tmp,							\
	     tmp = container_of_sm(pos->member.next, tmp, member))


/* append @other to @list */
static inline void
list_append_list(list_t *list, list_t *other)
{
	if (!other)
		return;
	list->prev->next = other;
	other->prev->next = list;
	list_t *tmp = list->prev;//when I can remove this line...
	list->prev = other->prev;
	other->prev = tmp;
}

//swap should be safe if one and another are both valid
static inline void
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

//This code should be used pretty often when we want to switch views to the front.
//we may want to do something like pop-up-update, push-down-update to move the views
static inline void
list_swap_header_update(list_t **header, list_t *another)
{
	//always looks for the boundary problems
	if (!(*header)) {
		*header = another;
		return;
	}
	list_swap(*header, another);
	*header = another;
}

/**
 * insert an element at the end of a list,
 */
static inline void
list_append_elem(list_t *header, list_t *elem)
{
	//append a elem to the the list_t is actually just insert it before header
	header->prev->next = elem;
	elem->prev = header->prev;
	header->prev = elem;
	elem->next = header;
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

static inline void
list_init(list_t *list)
{
	list->prev = list;
	list->next = list;
}

//insert after the elem
static inline void
list_insert(list_t *list, list_t *elm)
{
	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}


static inline void
list_remove(list_t *elm)
{
	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = NULL;
	elm->prev = NULL;
}

static inline int
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

static inline int
list_empty(const list_t *header)
{
	return header->next == header;
}

static inline void
list_insert_list(list_t *header, list_t *other)
{
	if (list_empty(other))
		return;

	other->next->prev = header;
	other->prev->next = header->next;
	header->next->prev = other->prev;
	header->next = other->next;
}


#ifdef __cplusplus
}
#endif

#endif /* EOF */
