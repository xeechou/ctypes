#include <stdlib.h>
typedef void *
	elemtype;
#include "lstack.h"

lstack initstack(void)
{
	lstack init = malloc(sizeof(*init));
	init->top = NULL;
}

void clearstack(lstack s) {
	while(s->top != NULL) {
		lnode tmp = s->top;
		s->top = s->top->last;
		free(tmp);
	}	
	free(s);
}
int isempty_ls(lstack s) { return (s->top == NULL) ? 0 : 1; }

void push(elemtype entry, lstack s) {
	lnode newnode = malloc(sizeof(*newnode));
	newnode->data = entry;
	newnode->last = s->top;
	s->top = newnode;
}

elemtype pop(lstack s) {
	elemtype entry = s->top->data;
	lnode tmp = s->top;
	s->top = s->top->last;
	free(tmp);
	return entry;
}
elemtype gettop(lstack s) {
	return s->top->data;
}
