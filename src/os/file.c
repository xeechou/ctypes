#include <stdio.h>
#include <unistd.h>
#include "file.h"

size_t
file_read(const char *f, char *mem, int max_size)
{
	FILE *stream = fopen(f, "r");
	size_t cr = ftell(stream);
	fseek(stream, 0, SEEK_END);
	size_t size = ftell(stream);
	fseek(stream, 0, SEEK_SET);
	int _size = (max_size > size+1) ? size : max_size - 1;
	if (mem)
		fread(mem, sizeof(char), _size, stream);
	mem[_size] = '\0';
	fseek(stream, cr, SEEK_SET);
	fclose(stream);
	return _size;
}

bool
is_dir_empty(DIR *d)
{
	if (!d)
		return false;
	int count = 0;
	struct dirent *dr;
	long curr = telldir(d);
	seekdir(d, 0);
	while ((dr = readdir(d)) != NULL) {
		if (++count >2)
			break;
	}
	seekdir(d, curr);
	return (count <= 2);
}
