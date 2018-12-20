#ifndef CTYPES_FILE_H
#define CTYPES_FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>


//avoid using helpers now, make them in the private namespace
size_t
file_read(const char *f, char *mem, int max_size)
{
	FILE *stream = fopen(f, "r");
	size_t cr = ftell(f);
	fseek(stream, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(stream, 0, SEEK_SET);
	int _size = (max_size > size+1) ? size : max_size - 1;
	if (mem)
		fread(mem, sizeof(char), _size, stream);
	mem[_size] = '\0';
	fseek(stream, cr, SEEK_SET);
	fclose(stream);
	return _size;
}



#endif
