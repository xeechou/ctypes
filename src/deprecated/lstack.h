#ifndef MYLSTACK_H
#define MYLSTACK_H	1

/* I havent define what elemtype is, 
 * define it in your own c source file */
struct linknode {
	elemtype data;
	struct linknode *
		last;
};

struct linkstack {
	struct linknode *top;
};
typedef struct linknode *lnode;
typedef struct linkstack *lstack;

/* Init a links stack and return the address */
extern lstack initstack(void);

extern void clearstack(lstack);
/* test if links stack is emtpy, return 0 if empty */
extern int isempty_ls(lstack);

extern void push(elemtype, lstack);
extern elemtype pop(lstack);
extern elemtype gettop(lstack);

#endif		/*endof lstack.h */
