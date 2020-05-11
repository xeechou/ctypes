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

#ifndef CTYPES_BUFF_H
#define CTYPES_BUFF_H

#include "../list.h"

#ifdef __cplusplus
#extern "C" {
#endif

/**
 * this buffer allocation strategy is basically assumming you won't free the
 * buffer unless you free all of them
 */
struct buff_list_t {
	list_t node;
	size_t size;
	off_t offset;
};

struct anonymous_buff_t {
	int fd;
	int size;
	list_t head;
	void *addr;
	int prot;
	int flags;
};

int anonymous_buff_new(struct anonymous_buff_t *buff, off_t size, int prot, int flags);

int anonymous_buff_alloc_by_offset(struct anonymous_buff_t *buff, off_t newsize);

void *anonymous_buff_alloc_by_addr(struct anonymous_buff_t *buff, off_t newsize);

int anonymous_buff_resize(struct anonymous_buff_t *buff, off_t size);

void anonymous_buff_close_file(struct anonymous_buff_t *buff);


#ifdef __cplusplus
}
#endif


#endif
