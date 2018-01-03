#include <stdlib.h>

//typedef int Qelemtype;
typedef void * Qelemtype;
#include "cirqueue.h"


/* initqueue: using system function to allocate memory for list */
cirqueue initqueue(int size, int ispointer) {
	cirqueue initone = (cirqueue)malloc(sizeof(*initone));
	initone->base = (Qelemtype *)malloc(sizeof(Qelemtype) * size);
	initone->front = 0;
	initone->back = -1;	// initial qlen = -1
	initone->ispntr = ispointer;
}


/* clearqueue: recycle memory of target queue */
void clearqueue(cirqueue q) {
	if (!q)
		return;
	if (!q->ispntr)
		free(q->base);
	else {
		while (q->front <= q->back) {
			free(q->base[front]);
			q->front += 1;
		}
	}
	free(q);
}


/* enqueue: try to insert a element at end of queue, if queue list not full */
/* arg[0] is data, arg[1] is queue name */
int enqueue(Qelemtype data, cirqueue q)  {
	if (!q || (q->back - q->front +1 >= q->maxsize))
		return -1;	//q is not defined, or overflow

	q->base[++(q->back) % q->maxsize] = data;
}


/* dequeue: try to serve a element at front of queue, if queue list not empty */
Qelemtype dequeue(cirqueue q) {
	//q empty, or q is NULL
	if (!q || q->back < q->front)	
		return NULL;
	if (q->ispntr)
		free(q->base[q->front]);
	q->front++;

}

/* quelength: return length of a queue */
int qlength(cirqueue q) {
	return (q) ? (q->back - q->front +1) : 0;
}


