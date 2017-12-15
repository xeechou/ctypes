typedef void * Telemtype;
#include "rb_tree.h"
#include <stdlib.h>
typedef rbnode Qelemtype;
#include "../lqueue.h"
/* Black Red tree keeps five property which we can search, insert, delete in O(lg(n)) 
 * 1: nodes are black or red;
 * 2: root is black;
 * 3: all the null leaf is black;
 * 4: Both children of every red node are black
 * 5: Every simple path from root to leaves contains the same number of black nodes. 
 */

rbtree rb_init(int ispointer) {
	rbtree init = malloc(sizeof(*init));
	init->root = NULL;
	init->ispntr = ispointer;
}
void rb_tree_destroy(rbtree tree) {
	if(!tree)
		return;
	lqueue treeq = initqueue(0);
	rbnode tr = tree->root;
	if (tr) {
		enqueue(tr, treeq);
	}
	while (qlength(treeq) > 0) {
		tr = dequeue(treeq);
		/* we want to know if we need to free data first */
		if (tree->ispntr) {
			free(tr->data);
			tr->data = NULL;
		}
		/* enqueue both tr's children*/
		if (tr->lsubtr)
			enqueue(tr->lsubtr, treeq);
		if (tr->rsubtr)
			enqueue(tr->rsubtr, treeq);
		free(tr);
	}
	tr = NULL;
	clearqueue(treeq);
	free(tree);
}


/* Left-Rotate */
int left_rotate(rbtree tree, rbnode x) {

	rbnode y = x->rsubtr;
	/* For easy to understand */
	//alpha = x->lsubtr;	/*leave unchanged */
	rbnode beta  = y->lsubtr;	/* switch to x->rsubtr */
	//gamma  = y->rsubtr;	/* leave unchanged */
	
	/* change for beta */
	x->rsubtr = beta;
	if(beta)
		beta->parent = x;
	/****/

	y->parent = x->parent;
	/* check if x is the root */
	if(!(x->parent))
		tree->root = y;
	/*check  if x is the left son of the right */
       	else if(x == x->parent->lsubtr)
		x->parent->lsubtr = y;
	else
		x->parent->rsubtr = y;

	y->lsubtr = x;
	x->parent = y;
}

int right_rotate(rbtree tree, rbnode x) {
	rbnode y = x->lsubtr;
	/* For easy to understand */
	// alpha = y->lsubtr;		/*leave unchanged */
	rbnode beta  = y->rsubtr;		/*switch to x->lsubtr */
	// gamma = x->rsubtr;		/*leave unchanged */
	
	/* change for beta */
	x->lsubtr = beta;
	if(beta)
		beta->parent = x;

	y->parent = x->parent;
	/* check if x is the root */
	if(!x->parent)
		tree->root = y;
	/* check if x is the lsubtr or right subtr */
	else if(x == x->parent->lsubtr)
		x->parent->lsubtr = y;
	else
		x->parent->rsubtr = y;

	y->rsubtr = x;
	x->parent = y;
}

int rb_insert(rbtree tree, Telemtype entry) {
	if (!tree)
		return -1;		//tree is not defined
	rbnode inode = malloc(sizeof(*inode));
	inode->data = entry;
	inode->rsubtr = NULL;
	inode->lsubtr = NULL;
	inode->color  = RED;

	rbnode x = tree->root;
	rbnode y = NULL;		// if x is NULL, inode->parent = NULl
	int comp = 0;
	while (x) {
		y = x;
		comp = compare(entry, x->data);
		x = (comp < 0) ? x->lsubtr: x->rsubtr;
	}
	// parent link setled 
	inode->parent = y;
	/* check if tree is empty */
	if (!y) 
		tree->root = inode;
	
	else if (comp < 0) 
		y->lsubtr = inode;
	else
		y->rsubtr = inode;
	// root cannot be NULL now
	tree->root->parent = NULL;	/* root->parent be null */
	rb_insert_fix(tree, inode);
}
/* insert_fix doesnt change structure of tree itself, only left_rotate and right_rotate change structure */
int rb_insert_fix(rbtree tree, rbnode z) {
	rbnode root = tree->root;
	/* uncle */
	rbnode y;
	int caz;
	/* terminater point: z is root or z's parent is black */
	while(z->parent && z->parent->color == RED) {
		y = rb_uncle(z);

		// CASE 1:
		/* parent and uncle are the same color */
		
		if (y && y->color == RED) {			
			z->parent->color = BLACK;
			y->color = BLACK;
			z->parent->parent->color = RED;
			z = rb_grandp(z);
		}
		// if not CASE 1: it could be 4 sub cases
/* four cases:
 * parent at left hand side 
 * case 1: 
 *  			g  (b)                              	f (b)
 * 		      /   \				      /   \
 * 		    f (r)   u (b)		----->      z (r)   g(r)    
 * 		  /						     \
 * 		z (r)						      u (b)
 *			
 * case 2:
 *			g  (b)			------>         g (b)         ----->           z (b)
 *		      /   \				      /   \			     /   \
 *		    f (r)  u (b)			    z(r)   u (b)		   f (r)   g (r)
 *		      \					   /                                        \
 *		       z(r)				 f (r)                                	      u (b)
 *
 * parent at right hand side
 * case 3:
 *                     g (b) 			------->       g (b)         -------->        z (b)   
 *                   /   \                                   /  \			    /   \
 *                 u (b)   f (r)			   u (r)  z (r)			  g (r)  f (r)
 *                       /  				  	    \                   /
 *                     z (r)         	       			     f (r)             u (b)
 *                     
 * case 4:
 *                     g (b) 			------->       f (b)
 *                   /   \                                   /  \
 *                 u (b)   f (r)			   g (r)  z (r)
 *                          \				 /    
 *                            z (r)         	       u (b)
 *
 *
 */
		/* uncle are black*/
		else {
			caz = rotate_case(tree, z, z->parent);
			switch(caz) {
				case 1: 	// z on left, parent on left
				z->parent->color = BLACK;
				rb_grandp(z)->color = RED;
				right_rotate(tree, rb_grandp(z));
				break;

				case 2:        //  z on right, parent on left
				z = z->parent;
				left_rotate(tree, z);
				z->parent->color = BLACK;
				rb_grandp(z)->color = RED;
				right_rotate(tree, rb_grandp(z));
				break;
				
				case 3:		// z on left, parent on right
				z = z->parent;
				right_rotate(tree, z);
				z->parent->color = BLACK;
				rb_grandp(z)->color = RED;
				left_rotate(tree, rb_grandp(z));
				break;

				case 4:		// z on right, parent on right
				z->parent->color = BLACK;
				rb_grandp(z)->color = RED;
				left_rotate(tree, rb_grandp(z));
				break;
			}
		}
	}
	tree->root->color = BLACK;				//BUG track: after rotate, root changed
}
/* the function tries to figure out if in CASE 1 CASE 2, or CASE 3 CASE 4 */
int rotate_case(rbtree tree, rbnode z, rbnode p) {
	/* bug track: if p is root, then it will not have parent */
	int caz;			//caz means case
	if (!p->parent) 
		caz = 1;		//set any value
	else 
		caz = (p == p->parent->lsubtr) ? 1 : 3;
	caz = (z == p->lsubtr) ? caz : caz+1;
	return caz;
}


Telemtype rb_delete(rbtree tree, Telemtype entry) {
	rbnode tr = tree->root;
	rbnode delnode = NULL;
	Telemtype ret;
	
	int comp;
	while (tr) {
		comp = compare(entry, tr->data);
		switch (comp) {
			case 0:		//find it, there could be still a node contains same data
			delnode = tr;
			tr = NULL;
			break;

			case -1:
			tr = tr->lsubtr;
			break;
			
			case  1:
			tr = tr->rsubtr;
			break;
		}
	}
	if (!delnode)			// we couldnt find the right node to delete
		return NULL;

	else {
		ret = rb_remove(tree, delnode);
	}
	return ret;
}
/* rb_remove: rmove a node
 * we at this function means we truly need delete a node */
Telemtype rb_remove(rbtree tree, rbnode z) {
	rbnode delnode;
	rbnode descends;			// represent descendants 

	Telemtype ret = NULL;
	if (!(z->lsubtr) || !(z->rsubtr))	//z has 0 or 1 subtr, z is the node to del
		delnode = z;	
	else 					//z has two child, z is not the node we want to del, we del its successor
		delnode = rb_successor(z);

	if (delnode->lsubtr)			// Now we have convert the problem to delete a node that has zero or one child
		descends = delnode->lsubtr;
	else
		descends = delnode->rsubtr;
	
	if(descends)				// if there is any descendant
		descends->parent = delnode->parent;
	if(delnode == tree->root)		// if we need to delete the root
		tree->root = descends;
	else if (delnode == delnode->parent->lsubtr)
		delnode->parent->lsubtr = descends;
	else
		delnode->parent->rsubtr = descends;

	if (delnode != z) {			// if z has two childs, we replace it's data with it's successor
		ret = z->data;
		z->data = delnode->data;
	}
	else 
		ret = delnode->data;

	if (delnode->color == BLACK && tree->root)			// bug found, tree is empty
		rb_delete_fix(tree, descends, delnode->parent);		// I add descends parent address here because descend could be NULL
	free(delnode);
	delnode = NULL;
	return ret;
}

/* x is the node that we delete its parent */
/* x could be now the root, or leaf, or usual node */

/* BUG track: x could be null, then we cannot find it's parent;
 * p is null: means that the whole tree is empty */

void rb_delete_fix(rbtree tree, rbnode x, rbnode p) {
/* Brief Idaa :
 * 	we delete x parent,
 * 	what used to be:	w is the uncle of x now becomes brother
 *		/ \ 						/ \
 *	       y   w  					--->   x   w
 *	      /   / \					      / \ / \
 *	     x
 *	we deleted y, and y is black, so we should add a black node on the road of x;
 *
 *	if x is red: just turns it black, we are okay;
 *
 *	if x is black: we need to bring a node from somewhere else to the road of x and turns it to red;
 *
 *	KEY IDEA: find a red node for X and turn it to black;
 */
	rbnode bro;
	rbnode rnephew;
	int caz;
	/* IN THE LOOP, x is always double black until we find a node for it */
	while (x != tree->root && rb_isblack(x) ) {	/* x could be NULL leaf at this time */
		bro = rb_brother(x, p);					
		caz = rotate_case(tree, x, p);
		//case one:		if brother is red, turn it black
		/*	  p (b)        ------>    w (b)			||		p (b)		------->	w (b)
		 *	/   \			/   \			||	      /   \			      /   \
		 *      x(db)   w (r)           p (r)  d (b)		||	    w (r)  x (db)		    c (b)   p (r)
		 *            /   \	    /   \			||        /   \				  	  /   \
		 *	  c (b)   d(b)	  x (db) c(b)			||   	c (b)   d (b)				d(b)    x(db)
		 *
		 */
		if (bro->color == RED) {				// if bro is red, parent must be black
			bro->color = BLACK;				// turn bro to black
			p->color = RED;
			/* if bro is right kid, left rotate, else right_rotate */
			if (caz == 1 || caz == 3) 
				left_rotate(tree, p);			// turn parent to red 
			else
				right_rotate(tree, p);
			/* p remains still */
			bro = rb_brother(x, p);				
		}
		//case 2:
		/* brother is black : if both kids are black  
		 * because x is double black, so there must be at least two black node on down the road where brother at */
		if (rb_isblack(bro->lsubtr) && rb_isblack(bro->rsubtr)) {
			bro->color = RED;	// bro must exsit
			x = p;			// x could still be a NULL node
		}
		// case 3: if not both brother's kids black, there must be one red
		/*	we didnt set p to any color means that p can be any color
		 *            p			------>		p			------>    c 
		 *          /   \			      /   \				 /   \
		 *        x(db)  w (b)			    x(db)  c (b)		       p (b)   w (b)
		 *             /   \				 /   \			     /           \
		 *           c (r)  n (b)			       w (r)		   x (b)	   n (b)
		 *          / 					         \
		 *           					           n (b)
		 *            p			------>            w 
		 *          /   \		              	 /   \
		 *        x(db)  w (b)		               p (b)   w (b)
		 *             /   \		             /   \       \
		 *           c      n (r)	           x (b)   c	   n (b)
		 *          / 			       
		 *           			       
		 *	the other two cases x is on the right hand side
		 */
		else {					// get nephew
			if (bro->lsubtr && bro->lsubtr->color == RED)
				rnephew = bro->lsubtr;
			else
				rnephew = bro->rsubtr;
			caz = rotate_case(tree, rnephew, bro);
			// p remains
			switch (caz) {
				case 1:		// bro left, nephew left
				bro->color = bro->parent->color;
				bro->parent->color = BLACK;
				rnephew->color = BLACK;			//nephew didnt changed
				right_rotate(tree, p);
				x = tree->root;
				break;
				case 2:					// bro left, nephew right
				rnephew->color = BLACK;
				bro->color = RED;
				left_rotate(tree, bro);
				bro = p->lsubtr;
				rnephew = bro->lsubtr;			//nephew changed
				
				bro->color = bro->parent->color;
				bro->parent->color = BLACK;
				rnephew->color = BLACK;			//nephew changed
				right_rotate(tree, p);
				x = tree->root;

				break;
				case 3:					// bro right, nephew left
				rnephew->color = BLACK;
				bro->color = RED;
				right_rotate(tree, bro);
				bro = p->rsubtr;
				rnephew = bro->rsubtr;

				bro->color = bro->parent->color;
				bro->parent->color = BLACK;
				rnephew->color = BLACK;
				left_rotate(tree, p);
				x = tree->root;
				break;
				case 4:					// bro right, nephew right
				bro->color = bro->parent->color;
				bro->parent->color = BLACK;
				rnephew->color = BLACK;
				left_rotate(tree, p);
				x = tree->root;
				break;
			}

		}
	}
	x->color = BLACK;
}
/*toolkit function*/
rbnode inline rb_grandp(rbnode n) {
	return n->parent->parent;
}

rbnode inline rb_uncle(rbnode n) {
	rbnode ret = n->parent->parent->rsubtr;

	if (n->parent == n->parent->parent->rsubtr)
		ret = n->parent->parent->lsubtr;
	return ret;
}

rbnode inline rb_brother(rbnode n, rbnode parent) {
	rbnode ret = parent->lsubtr;
	if (parent->lsubtr == n)
		ret = parent->rsubtr;

	return ret;
}

int inline rb_isblack(rbnode n) { return (!n || n->color == BLACK) ? 1 : 0; }

rbnode rb_successor(rbnode node) {
	rbnode successor = NULL;
	if (node->rsubtr) {
		successor = node->rsubtr;
		while(successor->lsubtr)
			successor = successor->lsubtr;
	}
	else {
		successor = node->parent;
		while (successor != NULL && node == successor->rsubtr) {	 // if node has a successor, it could be a acenstor, we go up, until we find a node
			node = successor;				// that node is the lsubtr of it, then it could be a successor
			successor = successor->parent;
		}
	}
	return successor;
}
