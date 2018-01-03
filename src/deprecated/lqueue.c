#include <stdlib.h>
typedef void * Qelemtype;	//set to general pointer
#include "lqueue.h"

lqueue initqueue(int ispointer) {
	lqueue init = (lqueue)malloc(sizeof(*init));
	init->head = NULL;
	init->rear = NULL;
	init->qlen = 0;
	init->ispntr = ispointer;
}

void clearqueue(lqueue q){
	lqnode tmp;
	/* I have to say bugs every where, if we just declare it and we havent define it, abs there is a segamentation fault */
	while (q->qlen > 0 ) {
		tmp = q->head;
		q->head = q->head->next;
		if (q->ispntr) {
			free(tmp->data);
			tmp->data = NULL;
		}
	}
	free(q);

}
/* Insertion operation is abs done in O(1) */
int enqueue(Qelemtype data, lqueue q) {
	lqnode last = malloc(sizeof(*last));
	last->data = data;
	last->next = NULL;

	/* if q empty */
	if (q->head == NULL ) {
		q->head = q->rear = last;
	}
	/* if queue not empty */
	else {
		q->rear->next = last;
		q->rear = last;
	}
	q->qlen += 1;
}
/* dequeue only works if queue not empty,
 * use qlength to guarantee the correctness */

/* O(1) */
Qelemtype dequeue(lqueue q) {
	if (q->qlen <= 0) {
		return NULL;
	}
	Qelemtype ret = q->head->data;
	q->qlen -= 1;		/* qlen could decreased to 0 at this time */
	q->head = q->head->next;
	q->rear = (q->head) ? q->rear: NULL;
	return ret;
}

int qlength(lqueue q) {
	return q->qlen;
}
