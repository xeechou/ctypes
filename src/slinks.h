struct lnode
 {
	 Lelemtype data;
	 struct lnode *next;
 };
typedef struct lnode *slink;	/*single link*/

extern slink linkinit(void);
extern int printlink(slink);
extern void clearlink(slink);

/* Insert after:
 * position ranges [0, Inf)*/
extern int insert_after_ith(slink, int, Lelemtype);

/* Insert before:
 * position ranges [1, Inf) */
extern int insert_before_ith(slink, int, Lelemtype);

/* Insert funtion: insert a node at right position,
 * the position ranges [1, Inf) */
extern int insert(slink, int, Lelemtype);

extern int del_node(slink, int);

extern int isempty_sl(slink);

/* user function */
extern void printnode(Lelemtype);
