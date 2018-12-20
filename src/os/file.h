#ifndef CTYPES_FILE_H
#define CTYPES_FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>

//file related operations

size_t file_read(const char *f, char *mem, int max_size);

bool is_dir_empty(DIR *dir);

static inline bool
is_file_exist(const char *abs_path)
{
	FILE *f;
	if ((f = fopen(abs_path, "r")) != NULL) {
		fclose(f);
		return true;
	}
	return false;
}


static inline struct dirent *
dir_find_pattern(DIR *dir, const char *format, ...)
{
	struct dirent *drnt = NULL;

	while ((drnt = readdir(dir)) != NULL) {
		va_list ap;
		int encounter = 0;
		va_start(ap, format);
		encounter = vsscanf(drnt->d_name, format, ap);
		va_end(ap);
		if (encounter != 0)
			break;
	}
	return drnt;
}


#endif
