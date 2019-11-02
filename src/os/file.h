#ifndef CTYPES_FILE_H
#define CTYPES_FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>

//file related operations

size_t file_read(const char *f, char *mem, int max_size);

bool is_dir_empty(DIR *dir);

int mkdir_p(const char *path, mode_t mode);

static inline bool
is_file_exist(const char *abs_path)
{
	FILE *f;
	if ((f = fopen(abs_path, "r")) != NULL) {
		fclose(f);
		return true;
	}
	errno = 0;
	return false;
}

static inline bool
is_dir_exist(const char *abs_path)
{
	DIR *d;
	if ((d = opendir(abs_path)) != NULL) {
		closedir(d);
		return true;
	}
	errno = 0;
	return false;
}

static inline bool
is_file_type(const char *f, const char *niddle)
{
	return strstr(f, niddle) + strlen(niddle) ==
		f + strlen(f);
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

static inline char *
path_concat(char *path, int max_len, int narg, ...)
{
	va_list ap;

	va_start(ap, narg);
	for (int i = narg; i > 0; i--) {
		if (strlen(path) >= max_len)
			break;
		const char *node =
			va_arg(ap, const char *);
		strcat(path, "/");
		strcat(path, node);
	}
	va_end(ap);
	return path;
}


#endif
