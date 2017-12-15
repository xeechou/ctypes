#ifndef MYCIRQ_H
#define MYCIRQ_H	1
/* sequential implement of cirqueue */
/* Didint defien Qelemtype here, define in somwhere else */
typedef struct {
	Qelemtype *base;
	int front;
	int back;
	int maxsize;
	int ispntr;
} *cirqueue;




/* initqueue: using system function to allocate memory for list */
extern cirqueue initqueue(int, int);

/* clearqueue: recycle memory of target queue */
extern void clearqueue(cirqueue);

/* enqueue: try to insert a element at end of queue, if queue list not full */
extern int enqueue(Qelemtype, cirqueue);

/* dequeue: try to serve a element at front of queue, if queue list not empty */
extern Qelemtype dequeue(cirqueue);

/* quelength: return length of a queue */
extern int qlength(cirqueue);

#endif		/* end of cirqueue.h */
