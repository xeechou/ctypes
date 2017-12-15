#include <stdio.h>
#include <stdlib.h>
typedef char * Telemtype;
#include "rb_tree.h"
#define BUFSIZE 128

#include "printree.h"
char *getword(char **, char **);
int compare(char *, char *);
int main() {
	rbtree tree = rb_init(0);	// we dont want to delete the pointer
	/* get some words and put them in a list*/

	char *s = malloc(BUFSIZE * sizeof(*s));//"abc abd abe abf abg abh abi abj\n";//
	*fgets(s, BUFSIZE, stdin);
	//printf("%s\n", s);
	// assumming we get a list of words, and we want to put them in *strs
	
	Telemtype strs[BUFSIZE];
	// every strs[i] points to a address in array strings,
	// so if we wanna free, we need to free all strings together
	char *strings = malloc(BUFSIZE * sizeof(*strings)); // try to cp the test in here
	int i;
	for (i = 0; i < BUFSIZE; i++) {
		if ((strs[i] = getword(&s, &strings)) != NULL);
		else
			break;
	}
	/* well, last strs[i] is NULL */

	int j;
	for (j = 0; j < i; j++) {
		rb_insert(tree,strs[j]);
	}
	print_t(tree->root);
    printf("Inorder Traversal\n");
	rb_inordertl(tree);
    printf("Postorder Traversal\n");
	rb_psordertl(tree);
	rb_tree_destroy(tree);
	/*for (j = 0; j < i; j++) {
		rb_delete(tree, strs[j]);
	}*/
}

/* getwords: cp a line *s to *cp, set white space in *s to '\0' in *cp
 * input: **s: a string, contians one '\0';
 * 	  **cp: a empty stings;
 * ouput: **cp: a sting contains many '\0';
 */
char *getword(char **s, char **cp) {
	/* jump over white space first */
	while (**s == ' ' || **s == '\t')
		(*s)++;
	//save origin s;
	char *word;
	word = *cp;

	while (**s != '\n') {
		if ((**s >= 'a' && **s <= 'z') || (**s >= 'A' && **s <= 'Z'))
			*(*cp)++ = *(*s)++;	//cp the test
		if (**s == ' ' || **s == '\t') {
			**cp = '\0';		//use '\0' to separete every word
			(*cp)++;	//move to a new place for next time to stor things
			break;
		}
	}
	/* there are two case of end:
	 * a word followed a '\n' like "abc\n", or a blank chars followed \n
	 * but the difference is *s jumps blank first, so a *cp == word if just \n */
	return (*cp == word) ? NULL : word;
}


int compare(char *l, char *r) {
	while (*l && (*l == *r)) {
		l++;
	       	r++;
	}
	int comp = *l - *r;
	if (comp == 0);
	if (comp > 0)
		comp = 1;
	if (comp < 0)
		comp = -1;

	return comp;
}
