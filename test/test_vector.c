#include <string.h>
#include "../src/vector.h"


int main(int argc, char *argv[argc])
{
	vector_t v;
	vector_init(&v, sizeof(int), NULL);
	int i;
	for (i = 0; i < 100; i++)
		vector_append(&v, &i);
	vector_erase(&v, 50);
	int a = 1000;
	vector_insert(&v, &a, 50);
	vector_destroy(&v);
	return 0;
}
