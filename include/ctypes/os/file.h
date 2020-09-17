/*
 * Copyright (c) 2019 Xichen Zhou
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

bool
find_executable(const char *exe, char *fullpath, unsigned max_len);

static inline bool
is_file_exist(const char *abs_path)
{
	struct stat st;

	if (stat(abs_path, &st) < 0)
		return false;
	if (!S_ISREG(st.st_mode))
		return false;
	return true;
}

static inline bool
is_dir_exist(const char *abs_path)
{
	struct stat st;

	if (stat(abs_path, &st) < 0)
		return false;
	if (!S_ISDIR(st.st_mode))
		return false;
	return true;
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
	char *ret = path;

	va_start(ap, narg);
	for (int i = narg; i > 0; i--) {
		const char *node =
			va_arg(ap, const char *);
		if (strlen(path) + strlen(node) >= (unsigned)max_len-2) {
			ret = NULL;
			break;
		}
		strcat(path, "/");
		strcat(path, node);
	}
	va_end(ap);
	return ret;
}

static inline char *
path_join(char *path, int max_len, int narg, ...)
{
	va_list ap;
	char *ret = path;

	va_start(ap, narg);
	for (int i = narg; i > 0; i--) {
		const char *node =
			va_arg(ap, const char *);
		if (strlen(path) + strlen(node) >= (unsigned)max_len-2) {
			ret = NULL;
			break;
		}
		if (i == narg) {
			strcpy(path, node);
		} else {
			strcat(path, "/");
			strcat(path, node);
		}
	}
	va_end(ap);
	return ret;
}

#endif
