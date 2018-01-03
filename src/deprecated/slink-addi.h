
/*merge two sorted links in sorted order*/
slink mergelinks(slink l, slink r) {
	slink pl = l->next;
	slink pr = r->next;
	slink merge = l;

	while (pl && pr) {
		if (pl->data <= pr->data) {
			merge->next = pl;
			merge = pl;
			pl = pl->next;
		}
		else {
			merge->next = pr;
			merge = pr;
			pr = pr->next;
		}
	}
	merge->next = (pl) ? pl : pr;
	free(r);
	merge = l;
	return merge;

}



void reverse_link(slink l) {
	slink r = NULL;
	slink t , y = l->next;
	while (y != NULL) {
		t = y->next;
		y->next = r;
		r = y;
		y = t;
	}
	l->next = r;
}
