#ifndef MYRBTREE_H
#define MYRBTREE_H 	1

/* file of black red tree implementation */
typedef struct RBnode {
	int color;
	#define RED   1
	#define BLACK 0
	Telemtype data;
	struct RBnode *lsubtr;
	struct RBnode *rsubtr;
	struct RBnode *parent;
}*rbnode;

typedef struct RBtree {
	rbnode root;
	int ispntr;
}*rbtree;

extern rbtree rb_init(int);
extern void rb_tree_destroy(rbtree);

extern int rotate_case(rbtree, rbnode, rbnode); 
extern int left_rotate(rbtree, rbnode);

extern int right_rotate(rbtree, rbnode);

extern int rb_insert(rbtree, Telemtype);

extern int rb_insert_fix(rbtree, rbnode);

extern Telemtype rb_delete(rbtree, Telemtype);	//find node
extern Telemtype rb_remove(rbtree, rbnode);	//delete the node
extern void rb_delete_fix(rbtree, rbnode, rbnode);	//fix red black property
/* simple toolkit */
extern rbnode rb_grandp(rbnode);
extern rbnode rb_uncle(rbnode);
extern rbnode rb_brother(rbnode, rbnode);
extern rbnode rb_successor(rbnode);
extern int rb_isblack(rbnode);

/*extern function */
void rb_pstree(rbtree);
#endif
