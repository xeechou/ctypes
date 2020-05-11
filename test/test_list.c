#include <ctypes/list.h>

int main(int argc, char *argv[])
{
	struct int_list {
		list_t node;
		int elem;
	};

	struct int_list head;
	struct int_list head1;
	struct int_list a = {
		.elem = 1
	};
	struct int_list b = {
		.elem = 2
	};
	struct int_list c = {
		.elem = 3
	};
	struct int_list d = {
		.elem = 4
	};
	list_init(&head.node);
	list_init(&head1.node);
	printf("head0 is empty? %d\n", list_empty(&head.node));
	printf("head1 length? %d\n", list_length(&head1.node));
	list_insert(&head.node, &a.node); //working
	list_insert(&head.node, &b.node);
	list_append(&head1.node, &c.node); //working
	list_append(&head1.node, &d.node);
	struct int_list *itr;
	list_for_each(itr, &head.node, node) {
		printf("%d\t", itr->elem);
	}
	printf("\n");
	list_for_each(itr, &head1.node, node) {
		printf("%d\t", itr->elem);
	}
	printf("\n");
	list_insert_list(&head.node, &head1.node);
	list_for_each(itr, &head.node, node) {
		printf("%d\t", itr->elem);
	}
	printf("\n");

	return 0;
}
