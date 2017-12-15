#include <stdio.h>
/* This file is about tree traversal, pstree and something else */
typedef char * Telemtype;
#include "rb_tree.h"
#include <stdlib.h>
//void printab(int count);
/* ps tree will create a new tree which stores some different information about the tree and print it, and distroy it */
/*
void rb_pstree(rbtree tree) {
	lstack treestack = initstack();
	rbnode tr = tree->root;
	int level = 0;
	elemtype levelinfo = NULL;
	while (tr || isempty_ls(treestack)) {
		if (tr) {
			printf("%s\t", tr->data);//stands for a word
			if (tr->rsubtr) {
				levelinfo = malloc(sizeof(*levelinfo));
				levelinfo->level = level;
				levelinfo->node = tr;
				push(levelinfo, treestack);
			}
			tr = tr->lsubtr;
			level++;
		}
		else {
			levelinfo = pop(treestack);	//parent printed already be printed
			tr = levelinfo->node;
			level = levelinfo->level;
			free(levelinfo);
			levelinfo = NULL;
			putchar('\n');
			printab(level+1);			//null or not, the node must be at this level
			tr = tr->rsubtr;
		}
	}
	putchar('\n');
}
*/
void rb_pstree(rbtree tree) {
	
}
void printab(int count) {
	while(count-- > 0)
		putchar('\t');
}
/* traversal */
typedef struct {
	int isrecprob;
	rbnode node;
} *elemtype;
#include "../lstack.h"

void rb_inordertl(rbtree tree) {
	lstack trstack = initstack();
	elemtype top = NULL;

	if(tree->root) {
		top = malloc(sizeof(*top));
		top->node = tree->root;
		top->isrecprob = (top->node->lsubtr || top->node->rsubtr) ? 1:0;
		push(top, trstack);

	}
	rbnode tmp;
	elemtype lprob;		/* left  problem */
	elemtype rprob;		/* right problem */
	while(isempty_ls(trstack)){
		top = pop(trstack);
		tmp = top->node;

		if (!top->isrecprob) {
			printf("%s\t", top->node->data);
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
	putchar('\n');
	clearstack(trstack);
	trstack = NULL;		//avoid hangling pointer
	
}
void rb_prordertl(rbtree tree) {
	elemtype top = NULL;
	lstack trstack = initstack();
	if (tree->root) {
		top = malloc(sizeof(*top));
		top->isrecprob = (tree->root->lsubtr || tree->root->rsubtr) ? 1 : 0;
		top->node = tree->root;
		push(top, trstack);
	}
	rbnode tmp;
	elemtype lprob;		/* left  problem */
	elemtype rprob;		/* right problem */
	
	while (isempty_ls(trstack)) {
		top = pop(trstack);
		tmp = top->node;
		if (!top->isrecprob) {
			printf("%s\t", tmp->data);
			free(top);
		}
		else {
			if(tmp->rsubtr) {
				rprob = malloc(sizeof(*rprob));
				rprob->node = tmp->rsubtr;
				rprob->isrecprob = (tmp->rsubtr->lsubtr || tmp->rsubtr->rsubtr) ? 1 : 0;
				push(rprob, trstack);
			}
			if (tmp->lsubtr) {
				lprob = malloc(sizeof(*rprob));
				lprob->node = tmp->lsubtr;
				lprob->isrecprob = (tmp->lsubtr->lsubtr || tmp->lsubtr->rsubtr) ? 1 : 0;
				push(lprob, trstack);
			}
			top->isrecprob = 0;
			push(top, trstack); 
		}
	}
	putchar('\n');
	clearstack(trstack);
	trstack = NULL;
}

void rb_psordertl(rbtree tree) {
	lstack trstack = initstack();
	elemtype top = NULL;
	if (tree->root) {
		top = malloc(sizeof(*top));
		top->node = tree->root;
		top->isrecprob = (top->node->lsubtr || top->node->rsubtr) ? 1 : 0;
		push(top, trstack);
	}
	rbnode tmp;
	elemtype lprob;		/* left  problem */
	elemtype rprob;		/* right problem */
	while (isempty_ls(trstack)) {
		top = gettop(trstack);
		tmp = top->node;
		if (!top->isrecprob) {
			printf("%s\t",top->node->data);
			pop(trstack);
			free(top);
		}
		else {
			// visit in LRV order, push V first, push R second, push L last
			top->isrecprob = 0;	//no need to push it again;	

			//push rsubtr
			if (tmp->rsubtr) {
				rprob = malloc(sizeof(*rprob));
				rprob->node = tmp->rsubtr;
				rprob->isrecprob = (tmp->rsubtr->lsubtr || tmp->rsubtr->rsubtr) ? 1 : 0;
				push(rprob, trstack);
			}
			if (tmp->lsubtr) {
				lprob = malloc(sizeof(*lprob));
				lprob->node = tmp->lsubtr;
				lprob->isrecprob = (tmp->lsubtr->lsubtr || tmp->lsubtr->rsubtr) ? 1 : 0;
				push(lprob,trstack);
			}
		}
	}
	putchar('\n');
	clearstack(trstack);
	trstack = NULL;
}

