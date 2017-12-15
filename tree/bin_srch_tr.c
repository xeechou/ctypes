/* This file is about binary search tree implement */
#include <stdio.h>
#include <stdlib.h>
typedef void* Telemtype;
#include "bintree.h"

/* error summary: In this file, we seems to use 0 as return value once tree is empty*/
/****************************************************
 *
 * basic operations begins here (listed in head file)
 *
 ****************************************************/

/* generate a root node for a binary tree */
bintree gen_root(int ispointer) {
	bintree init = malloc(sizeof(*init));
	init->ispntr = ispointer;
	init->root = NULL;

	return init;
	/* just NULL address for root */
}

/* del_tree: delete a tree
 * lazy soul edition */
void del_tree(bintree tr) {
	rm_tree(tr->root, tr->ispntr);
	/* this may leave a dangerous opertion here, when we try to visit tr after we free tr*/
	free(tr);

}
void rm_tree(btnode tr, int ispointer) {
	if(!tr);
	else {
		rm_tree(tr->lsubtr, ispointer);
		rm_tree(tr->rsubtr, ispointer);
		if(ispointer)
			free(tr->data);
		free(tr);
	}
}

/* compare : the function try to decide a way to compare two elem
 * return 0 : l == r;
 * return 1 : l >  r;
 * return -1: l <  r;
 * metioned: this function shouled be created by user himself 
 * because we cannot decide what type Telemtype is, then we cannot decide how to compare two element */
/* int compare(Telemtype l, Telemtype r) {
	
	// assume that Telemtype is int 
	int val = l - r;
	if (val == 0)
		return 0;
	if (val < 0 )
		return -1;
	return 1;
} */

/* lookup: using compare function to find out if entry is already in the tree,
 * return address of node if found, return NULL otherwise */
btnode  lookup(bintree tree, Telemtype entry) { 
	int comp;					// comp: result of comparison	
	btnode tr = tree->root;
	while (tr) {
		/*compare function, 1: >; -1: <; 0: = */
		comp = compare(entry, tr->data);
			if(!comp)
				break;
			tr = ((comp) == 1) ? tr->rsubtr : tr->lsubtr;
	}
	/* failed to find, tr == NULL anyway */
	return tr;
}

/* addleaf: insert tree node at a right position */
void addleaf(bintree tree, Telemtype entry) {

	int comp;
	btnode tr = tree->root;
	btnode prenode = tr;
	while (tr){
		prenode = tr;
		comp = compare(entry, tr->data);
		if (comp < 0) {
			tr = tr->lsubtr;
		}
		else  {
			tr = tr->rsubtr;
		}
	} 
	tr = malloc(sizeof(*tr));
	tr->data = entry;
	tr->lsubtr = NULL;
	tr->rsubtr = NULL;

	if (prenode) {
		prenode->lsubtr = (comp <  0) ? tr : prenode->lsubtr;
		prenode->rsubtr = (comp >= 0) ? tr : prenode->rsubtr;
	}
	else {
		tree->root = tr;
	}
	
}

/* addleaf_dis: add a distinct leaf in a tree, if tree is not empty */
void addleaf_dis(bintree tree, Telemtype entry) {
	btnode tr = tree->root;
	int comp = 0;
	btnode prenode = tr;		/* if root is empty, prenode == tr == root == NULL */

	while (tr) {
		prenode = tr;
		comp = compare(entry, tr->data);
		switch(comp) {
			case 0:
			/* help to jump out of loop */
			tr = NULL;
			break;
			case 1:
			tr = tr->rsubtr;
			break;
			case -1:
			tr = tr->lsubtr;
			break;
		}
	}
	/* if comp is not 0, we need insert a new node */
	if (comp || !prenode) {
		tr = malloc(sizeof(*tr));
		tr->data = entry;
		tr->lsubtr = NULL;
		tr->rsubtr = NULL;
	}
	/* if prenode is not empty, we can certainly insert a node at tr */
	if (prenode) {
		prenode->lsubtr = (comp == -1) ? tr : NULL;
		prenode->rsubtr = (comp ==  1) ? tr : NULL;
	}
	/* last case, tree is a empty tree itself, prenode = tr */
	else {
		tree->root = tr;
	}
}
/* delsubtr: delete subtree
 * return -1 if we didnt find node */
int del_subtr (bintree tree, Telemtype entry) {

	int comp;
	btnode tr = tree->root;
	btnode parent = tr;
	while(tr) {
		comp = compare(entry, tr->data);
		switch (comp) {
		case 0:
			rm_tree(tr,tree->ispntr);
			if (parent) {					//change parent node 
				parent->lsubtr = (parent->lsubtr == tr) ? NULL : parent->lsubtr;
				parent->rsubtr = (parent->lsubtr == tr) ? NULL : parent->rsubtr;
			}
			/* delete completed */
			return 0;
			break;
		case 1:
			tr = tr->rsubtr;
			break;
		case -1:
			tr = tr->lsubtr;
			break;
		}
	}
	/* Could not find exact point, return 0 */
	return -1;
}
/* del_node : del a node of binary search tree, if didnt find, return -1, doesn't change subtr of node to del */
int del_node(bintree tree, Telemtype entry) {
	btnode tr = tree->root;
	btnode delnode = NULL;
	btnode parent = NULL;
	int comp;
	while (tr) {					// find node to del, if not, delnode is NULL 
		comp = compare(entry, tr->data);
		switch(comp) {
			case -1:
				parent = tr;
				tr = tr->lsubtr;
				break;
			case  1:
				parent = tr;
				tr = tr->rsubtr;
				break;
			case  0:
				delnode = tr;
				tr = NULL;
				break; 
		}
	}
	/* found node to del or tree is empty*/

	/* tree is empty */
	if (delnode == NULL)				// nothing to del, or we didnt find 
		return -1;
	// truly there is a node to delete, we call it delnode 
	/* if delnode has 0 or 1 kid */
	else if (!(delnode->lsubtr) || !(delnode->rsubtr));	
	/* delnode has two sons */
	else {
		/* find successor */
		tr = delnode;
		btnode successor = delnode->rsubtr;
		while (successor->lsubtr) {
			parent = successor;
			successor =  successor->lsubtr;
		}
		delnode = successor;
		successor = tr;
		/*found successor */
	}
	if (delnode->lsubtr)
		tr = delnode->lsubtr;				// tr is descendant 
	else
		tr = delnode->rsubtr;

	if (delnode == tree->root) {				// if we wanna delete the root
		tree->root = NULL;
	}
	else if (parent->lsubtr = delnode)
		parent->lsubtr = tr;
	else
		parent->rsubtr = tr;

	if (delnode == successor) { 				// if we are in case 3
		if(tree->ispntr) {
			free(successor->data);
			successor->data = delnode->data;
		}
		else
			free(delnode->data);
	}
	free(delnode);
}
/******************
 * traversal begins
 * ****************/
typedef struct {
	int isrecprob;			// is a recursive problem ?
	btnode node;
} *elemtype;
#include "../lstack.h"

/* visit node, a simple example, could be a much bigger one if you define yourself */
void visit(btnode node) {
	printf("%d\n", node->data);
}
/* inorder traversal
 * LVR */
int inordertl(bintree tree) { 	/* tr represents tree */
	lstack trstack = initstack();
	elemtype top;

	if (tree->root) {
		btnode tr = tree->root;

		top = malloc(sizeof(*top));
		top->node = tr;
		top->isrecprob = (tr->lsubtr || tr->rsubtr) ? 1 : 0;
		push(top, trstack);
	}
	
	btnode tmp;

	elemtype lprob;		/* left  problem */
	elemtype rprob;		/* right problem */
	
	while(isempty_ls(trstack)){
		top = pop(trstack);
		tmp = top->node;

		if (!top->isrecprob) {
			visit(tmp);
			free(top);
		}
		/* push R first, V second, L last, just as the inverse of LVR */
		else {
			if (tmp->rsubtr) {
				rprob = malloc(sizeof(*rprob));
				rprob->node = tmp->rsubtr;
				rprob->isrecprob = (tmp->rsubtr->lsubtr || tmp->rsubtr->rsubtr) ? 1 : 0;
				push(rprob, trstack);
			}

			top->isrecprob = 0;	// next time we just vist it and done 
			push(top, trstack);

			if (tmp->lsubtr) {
				lprob = malloc(sizeof(*rprob));
				lprob->node = tmp->lsubtr;
				lprob->isrecprob = (tmp->lsubtr->lsubtr || tmp->lsubtr->rsubtr) ? 1 : 0;
				push(lprob, trstack);
			}

		}
	} 
}

/* breadth first: queue technique */

typedef btnode qelemtype;
#include "../cirqueue.h"
#define MAXTREE_TMP	128
void breadtl(bintree tree) {
	cirqueue treequeue = initqueue(MAXTREE_TMP);
	btnode tr =  tree->root;
	if (tr)
		enqueue(treequeue, tr);

	while (qlength(treequeue) > 0) {
		tr = dequeue(treequeue);
		visit(tr->data);
		if (tr->lsubtr)
			enqueue(treequeue, tr->lsubtr);
		if (tr->rsubtr)
			enqueue(treequeue, tr->rsubtr);
	}
}


