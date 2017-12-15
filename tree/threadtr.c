/* Demo for threaded tree, provide limite functions*/

#include <stdio.h>
#include <stdlib.h>
typedef int Telemtype;
typedef struct threadtr {
	Telemtype data;
	struct threadtr *lsubtr;
	struct threadtr *rsubtr;
	int tag;
} *thrtr;
/* gen_root: return a Null address for root */
thrtr gen_root(void) {
	return NULL;
}
int insrtnd(thrtr, Telemtype);
/* gen_tree: given a set, use compare function to generate a thtreaded tree */
thrtr gen_tree(Telemtype *set, int n) {

	thrtr root = malloc(sizeof(*root));
	root->lsubtr = NULL;
	root->rsubtr = NULL;
	root->tag = 0;
	root->data = *set++;
	n--;
	
	if (n < 1)
		return root;

	while (n-- > 0) {
		/*Insert Tree Node */
		insrtnd(root, *set);
		set++;
	}
}

int insrtnd(thrtr tree, Telemtype entry) {
	int compv;
	thrtr successor = NULL;
	while(tree) {
		compv = compare(entry, tree->data);
		switch (compv) {
		case 0:
			printf("Node exist.\n");
			return 1;
		case 1:

			successor = tree->rsubtr;
			(tree)->tag = 0;
			
			tree = tree->rsubtr;
			
			break;
		case -1:
			successor = tree;
			tree = tree->lsubtr;
			break;
		}

	}
	tree = malloc(sizeof(*tree));
	tree->lsubtr = NULL;
	tree->rsubtr = successor;
	tree->data = entry;
	tree->tag = 1;
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
/* Pre-order traversal */
int  pstree(thrtr tree) {
	int tabn = 0;
	if (!tree)
		return 0;
	while (tree || tree->rsubtr ) {
		if (tree->lsubtr) {
			printnode(tree->data, tabn++);
			tree = tree->lsubtr;
			continue;
		}
		/* tag on, means that the node is a leaf */
		if (tree->tag) {
			printnode(tree->data, tabn--);
			/* I must have visit tree->rsubtr */
			tree = tree->rsubtr;
			tabn = (tree->tag);
			continue;
		}
		else {
			putchar('\n');
			ptab(tabn);
			tree = tree->rsubtr;
		}
	}

}
int main() {
	Telemtype list []= { 5, 6, 8, 4, 2, 9, 1, 0, 3, 7};
	thrtr tree = gen_tree(list, 10);
}
