#ifndef CLQUEUE_H
#define CLQUEUE_H
#include <stdlib.h>


typedef struct linkq {
	Qelemtype data;
	struct linkq *next;
} *lqnode;

typedef struct {
	lqnode head;
	lqnode rear;
	int    qlen;
	int    ispntr;
} *lqueue;


extern lqueue initqueue(int);
extern void clearqueue(lqueue);

extern int enqueue(Qelemtype, lqueue);
extern Qelemtype dequeue(lqueue);
extern int qlength(lqueue);
#endif
