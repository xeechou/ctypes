#ifndef MYBINTREE_H
#define MYBINTREE	1

/* binary tree data structure, naive one */

typedef struct Btreenode {
	/* indicate what the Telemtype is */
	/* ispointer == 0: char
	 * ispointer == 1: pointer, point to somewhere else */
	Telemtype data;
	//int nodenum;
	struct Btreenode *lsubtr;	/* left  subtree */
	struct Btreenode *rsubtr;	/* right subtree */
} *btnode;
typedef struct {
	int ispntr;	/* Telemtype Is pointer ? */
	btnode root;
} *bintree;
/* there is a problem for tree: what do I store on a tree? 
 * If a tree just store a name.
 * then It's gonna be useless. */
/* generate a root node for a binary tree */
extern bintree gen_root(int);
extern int isemtpy_tr(bintree);



/* delete a tree for some reason */
extern void del_tree(bintree);
extern void rm_tree(btnode, int);

/* Look up if the tree node exsist */
extern btnode lookup(bintree, Telemtype);


/* add a leaf, much like look up, no compare function declared in head file */
extern void addleaf(bintree, Telemtype);

/* add a leaf, if exist, then do nothing */
extern void addleaf_dis(bintree, Telemtype);

/* delete subtree: free a node if leaf, else, free a subtree */
extern int del_subtr(bintree, Telemtype);

/* delete node: free a node, change structure of tree if node is not leaf */
/* if Telemtype is not there, return -1 */
extern int del_node(bintree, Telemtype);

/*******************************
 * travesal tree in some order 
 *******************************/

/* depth first */

/* inorder traversal */
extern int inordertl(bintree);

/* preorder traversal */
//extern int prordertl(bintree);

/* postorder traversal */
//extern int psordertl(bintree);
/* breadth first */
extern void breadtl(bintree);

#endif		/* end of bintree.h */
