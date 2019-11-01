#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <errno.h>
#include <sys/stat.h>
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


int mkdir_p(const char *path, mode_t mode)
{
	const size_t len = strlen(path);
	char _path[PATH_MAX];
	char *p;
	struct stat buf;

	errno = 0;

	if (len > sizeof(_path)-1) {
		errno = ENAMETOOLONG;
		return -1;
	}
	strcpy(_path, path);
	for (p = _path+1; *p; p++) {
		if (*p == '/') {
			*p = '\0';

			if (mkdir(_path, S_IRWXU) != 0) {
				stat(_path, &buf);
				if (errno != EEXIST ||
				    (errno == EEXIST && !S_ISDIR(buf.st_mode)))
					return -1;
			}
			*p = '/';
		}
	}
	if (mkdir(_path, mode) != 0) {
		stat(_path, &buf);
		if (errno != EEXIST ||
		    (errno == EEXIST && !S_ISDIR(buf.st_mode)))
			return -1;
	}
	return 0;
}
