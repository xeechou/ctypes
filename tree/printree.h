/* these codes are mostly copied from stackoverflow.com 
 * thanks for their simple and beautiful code*/

#include <string.h>
int _print_t(rbnode tree, int is_left, int offset, int depth, char s[20][128])
{
    if (!tree) return 0;
    /* width means node length;
	 * left means left subtree string length
	 * right means right subtree string length */
    char b[20];
    int width = strlen(tree->data)+2;

    sprintf(b, "(%s)", tree->data);	//store in b

    int left  = _print_t(tree->lsubtr,  1, offset,                depth + 1, s);
    int right = _print_t(tree->rsubtr, 0, offset + left + width, depth + 1, s);
    int i;
#ifdef COMPACT
    for (i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];
	/* if not root and at left */
    if (depth && is_left) {	

        for (i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';
	/* if not root and at right */
    } else if (depth && !is_left) {

        for (i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
#endif

    return left + width + right;
}

int print_t(rbnode tree)
{
    int i;
    char s[20][128];
    for (i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _print_t(tree, 0, 0, 0, s);

    for (i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}
