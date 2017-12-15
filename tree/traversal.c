/* To illustrate the general picture of recurrence, do not use elegent method,
 * Recurrence, always include some situation that can be solved once, other situation, however, need to finish the job recursivly. */

/* I think, there is some basic classification for tree traversal, some point, we just visit it and we are done, some points, we cannot simply visit it */


/* we need a stack to store points we visit later */
typedef struct stackelem {
	int isubprob; 
	bintree node;
} elemtype;
int inordertl(bintree tr) {
	if (!tr)
		return NULL;
	
	lstack trstack = initstack();
	elemtype top = malloc(sizeof(*top));
	top->node = tr;
	top->isubprob = (tr->lsubtr || tr->rsubtr) ? 1 : 0;
	push(top, trstack);

	bintree tmp;
	int issubtr;
	elemtype lprob;
	elemtype rprob;
	
	do{
		top = pop(trstack);
		tmp = top->node;
		issubtr = top->isubprob;
		if (!issubtr) {
			visit(tmp);
			free(top);
		}
		else {
			if (tmp->rsubtr) {
				rprob = malloc(sizeof(*rprob));
				rprob->node = tmp->rsubtr;
				rprob->isubprob = (tmp->rsubtr->lsubtr || tmp->rsubtr->rsubtr) ? 1 : 0;
				push(rprob, trstack);
			}
			top->isubprob = 0;
			push(top, trstack);
			if (tmp->lsubtr) {
				lprob = malloc(sizeof(*rprob));
				lprob->node = tmp->rsubtr;
				lprob->isubprob = (tmp->lsubtr->lsubtr || tmp->lsubtr->rsubtr) ? 1 : 0;
				push(iprob, trstack);
			}

		}
	} while(isempty_ls(trstack));
}

int psordertl(bintree tr) {
	if (!tr) 
		return NULL;
	lstack trstack = initstack();
	elemtype top = malloc(sizeof(*top));
	top->node = tr;
	top->isubprob = (tr->lsubtr || tr->rsubtr) ? 1 : 0;
	push(top);

	bintree tmp;
	int issubtr;
	elemtype lprob;
	elemtype rprob;

	do {
		top = pop(trstack);
		issubtr = top->isubprob;
		tmp = top->node;
		if (!>isubtr) {
			visit(tmp);
			free(top);
		}
		else {
			top->isubprob = 0;
			push(trstack, top);
			
			if(tmp->rsubtr) {
				rprob = malloc(sizeof(*rprob));
				rprob->node = tmp->rsubtr;
				rprob->isubprob = (tmp->rsubtr-lsubtr || tmp->rsubtr->rsubtr) ? 1 : 0;
				push(rprob, trstack);
			}
			if(tmp->lsubtr) {
				lprob = malloc(sizeof(*lprob));
				lprob->node = tmp->lsubtr;
				lprob->isubprob = (tmp->lsubtr->lsubtr || tmp->lsubtr->rsubtr) ? 1 : 0;
				push(lprob, trstack);
			}
		}
	} while (isempty_ls(trstack));

}

