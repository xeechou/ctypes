/* xeechou@gmail.com */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
typedef  void *Lelemtype;
#include "slinks.h"

/*the function initialize a head node and return its' address*/
slink linkinit(void) {
	slink init_link = malloc(sizeof( *init_link));
	init_link->next = NULL;	/* If head node->next happen to be NULL, just indicate that this is a empty list */
}

/*the function print whole list except head node*/
int printlink(slink l) {
	slink p = l;
	if (p->next == NULL) {
		printf("null link list\n");
		return 0;
	}	
	p = p->next;
	while (p != NULL) {
		printnode(p->data);
		p=p->next;
	}	
	putchar('\n');
	return 0;
}

/*the function returns memory space back to system*/
void clearlink(slink l) {
	/* head node and data node, in this case, have no difference */
	while (l) {
		slink tmp = l;	
		l = l->next;
		free(tmp->data);
		free(tmp);
	}
}

/*the function insert into list after ith element, when i is greater than the list length or i <0, the insertion fails*/
int insert_after_ith(slink l, int i, Lelemtype elem) {
	/* l is a link with head node*/
	slink p = l;
	int j = 0;
	/*the extrem situation is that when i equal 0, we are gonna insert at the beginning of list*/
	while (p && j < i)
	 {
		p = p-> next;
		++j;
	 }
	/*extrem situation: 
	 * i < 0   : j > i;
	 * i > len : !p */
	if (!p || j>i) return -1;			/* error: insert at illegal position*/

	/*if not extrem, j = i, p is at most the last one*/
	slink insert_node = malloc(sizeof(*insert_node));
	insert_node->data = elem;
	insert_node->next = p->next;
	p->next = insert_node;
	return 0;	
}
/*the function insert into list before ith element, when i is greater than the list length or i <=0, the insertion fails*/
int insert_before_ith(slink l, int i, Lelemtype elem) {
	/*l is a link with head node*/
	slink p = l;
	int j = 0;
	while (p && j<i)
	 {
		p = p->next;
		++j;
	 }

	/*insert before 0th node is forbiden*/
	if (!p || j>i || i == 0) return -1;			/* error: insert at illegal position*/

	/*if not extrem, j must be i*/
	slink insert_node = malloc(sizeof(*insert_node));
	insert_node->data = p->data;
	p->data = elem;
	insert_node->next = p->next;
	p->next = insert_node;
	return 0;
}
/* insert in list at ith position */
int insert(slink l, int i, Lelemtype elem) {
	/* legal i: i > 0 */
	insert_after_ith(l, i-1, elem);
}

/*delete ith node from list l, return error message about process, when done, return 0*/
int del_node(slink l, int i) {
	slink p = l;
	int j =0;
	while (p && j < i-1) {
		p = p->next;
		++j;
	}
	/*delete head node is forbidened*/
	if (!p || i <= 0) return -2;			/*error: illegal delete position*/

	/*if not extrem situation, j must be i-1*/
	slink tmp = p->next;
	/*if tmp is 0x0, then program should return -2*/
	int flag = (tmp == NULL) ? -2 : 0;
	p->next = (flag == -2) ? NULL : tmp->next;	/*if tmp is null, which means ith node is null, the deletion fails, keep i-1 length*/
	return flag;
	free(tmp->data);
	free(tmp);
}



/* follow English tradition, return 0 when empty */
int isempty_sl(slink l) {
	return (l->next == NULL) ? 0 : 1;
}


