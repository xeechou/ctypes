/* This file is about binary search tree implement */

#include <stdlib.h>
typedef int Telemtype ;
#include "bintree.h"
typedef bintree elemtype ;
#include "../lstack.h"
#define MAXHIERARCHY 32

/* error summary: In this file, we seems to use 0 as return value once tree is empty*/


/****************************************************
 *
 * basic operations begins here (listed in head file)
 *
 ****************************************************/

/* generate a root node for a binary tree */
bintree gen_root(void) {
	/* just NULL address for root */
	return  NULL;
}

/* del_tree: delete a tree
 * lazy soul edition */
void del_tree(bintree tr) {
	/* if nulltree */
	if (!tr)
		return 0;	

	else {
		del_tree(tr->lsubtr);
		del_tree(tr->rsubtr);

		if (tr->ispointer)
			free(tr->data);
		free(tr);
	}
	/* if not syscalls error. */
	return 0;

}

/* compare : the function try to decide a way to compare two elem
 * return 0 : l == r;
 * return 1 : l >  r;
 * return -1: l <  r;
 * metioned: this function shouled be created by user himself 
 * because we cannot decide what type Telemtype is, then we cannot decide how to compare two element */
int compare(Telemtype l, Telemtype r) {
	
	/* assume that Telemtype is int */
	int val = l - r;
	if (val == 0)
		return 0;
	if (val < 0 )
		return -1;
	return 1;
}

/* lookup: using compare function to find out if entry is already in the tree,
 * return address of node if found, return NULL otherwise */
bintree  lookup(bintree tr, Telemtype entry) {
	if (!tr)
		/* null tree, cannot find */
		return NULL;

	/* lorr: left or right */
	int lorr;
	do {
		/* compare function 
		 * standard of compare, 1: >; -1: <; 0: = */
		lorr = compare(entry, tr->data)
			if(!lorr)
				return tr;
			tr = ((lorr) == 1) ? tr->rsubtr : tr->lsubtr;
	}while (tr);
	return 0;
}

/* addleaf: insert tree node at a right position */
int addleaf(bintree tr, Telemtype entry) {
	if (!tr) {
		tr = malloc(sizeof(*tr));
		tr->data = entry;
		tr->lsubtr = NULL;
		tr->rsubtr = NULL;
		/* I dont know why I have to return, maybe not */
		return 0;
	}
	do {
		if (compare(entry, tr->data) < 0)
			tr = tr->lsubtr;
		else 
			tr = tr->rsubtr;
	} while (tr) 
	tr = malloc(sizeof(*tr));
	tr->data = entry;
	tr->lsubtr = NULL;
	tr->rsubtr = NULL;
	return 1;
}
/* delsubtr: delete subtree
 * return 0 if we didnt find node */
int delsubtr (bintree tr, Telemtype entry) {
	if (!tr)
		/* No thing do delete */
		return 0;
	int comp;
	while(tr) {
		comp = compare(entry, tr->data);
		switch (comp) {
		case 0:
			del_tree(tr);
			/* delete completed */
			return 1;
			break;
		case 1:
			tr = tr->rsubtr;
			break;
		case 2:
			tr = tr->lsubtr;
			break;
		}
	}
	/* Could not find exact point, return 0 */
	return 0;
}
/******************
 * traversal begins
 * ****************/

/* Preorder Traversal: VLR */
int prordertl(bintree tr) {
	if (!tr)
		return NULL;

	//piont to the root;
	bintree tmp = tr;
	lstack trstack = initstack();

	do {
	/* everytime we face a tree node, we treat it as root */
		if (tmp) {
			visit(tmp);
			push(tmp, trstack);
			tmp = tmp->lsubtr;
		}
		else {
			tmp = pop(trstack);
			tmp = tmp->rsubtr;
		}
	} while (isempty_ls(trstack) || tmp);
	
}
/* Inorder Traversal: LVR */
int inordertl(bintree tr) {
	if (!tr) 
		return NULL;

	bintree tmp = tr;
	lstack trstack = initstack();

	do {
		if (tmp) {
			push(tmp, trstack);
			tmp = tmp->lsubtr;
		}
		else {
			tmp = pop(trstack);
			visit(tmp);
			tmp = tmp->rsubtr;
		}
	} while (isempty_ls(trstack) || tmp);
	
}
/* Postorder Traversal: LRV */
int psordertl(bintree) {
	if (!tr)
		return NULL;

	bintree tmp = tr;
	lstack trstack = initstack();
	bintree prenode = NULL;
	

}
/* pstree: print a tree on screen, maybe the hardest problem */
/* Best use inorder traversal to do that */
int pstree(bintree tr) {
	if (!tr)
		return 0;
	int counter = 0;
	bintree tmp = tr;
	while (1) {
		if (tmp) {
			pstrnode(tmp);
			printf("--------");
			counter++;
			push(tmp->rsubtr);
			tmp = tmp->lsubt;
		}
		else {
			putchar('\n');
			j
		}
	}
}
