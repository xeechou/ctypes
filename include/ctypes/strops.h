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

#ifndef CTYPES_STROPS_H
#define CTYPES_STROPS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief zero terminated strncpy
 */
static inline char *
strop_ncpy(char *dest, const char *src, size_t size)
{
	if (size == 0)
		return NULL;
	strncpy(dest, src, size-1);
	dest[size-1] = '\0';

	return dest;
}

static inline char *
strop_ltrim(char *line)
{
	char *ptr = line;
	while (*ptr && isspace(*ptr)) ptr++;
	return ptr;
}

static inline void
strop_rtrim(char *line)
{
	char *ptr = line + strlen(line) - 1;
	while(ptr >= line && *ptr && isspace(*ptr)) *ptr-- = '\0';
}

#ifdef __cplusplus
}
#endif

#endif /* EOF */
