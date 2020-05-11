/*
 * Copyright (c) 2020 Xichen Zhou
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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include <ctypes/list.h>
#include <ctypes/os/buffer.h>
#include <ctypes/os/os-compatibility.h>

int
anonymous_buff_new(struct anonymous_buff_t *buff, off_t size, int prot, int flags)
{
	buff->fd = os_create_anonymous_file(size);
	if (buff->fd < 0)
		return -1;
	list_init(&buff->head);
	buff->size = size;
	buff->addr = mmap(NULL, size, prot, flags, buff->fd, 0);
	buff->prot = prot;
	buff->flags = flags;
	return buff->fd;
}

int
anonymous_buff_resize(struct anonymous_buff_t *buff, off_t size)
{
	if (ftruncate(buff->fd, size) < 0)
		return 0;

	munmap(buff->addr, buff->size);
	buff->addr = mmap(NULL, size, buff->prot, buff->flags, buff->fd, 0);
	buff->size = size;
	return size;
}



int
anonymous_buff_alloc_by_offset(struct anonymous_buff_t *buff, off_t newsize)
{
	struct buff_list_t *itr;
	off_t allocated = 0;
	list_for_each(itr, &buff->head, node) {
		allocated += itr->size;
	}
	if ((int)buff->size - (int)allocated  < newsize)
		if (!anonymous_buff_resize(buff, 2 * buff->size + newsize))
			return -1;

	//otherwise, we can do the insert
	itr = malloc(sizeof(*itr));
	itr->size = newsize;
	itr->offset = allocated;
	list_append(&buff->head, &itr->node);
	return itr->offset;
}

void *
anonymous_buff_alloc_by_addr(struct anonymous_buff_t *buff, off_t newsize)
{
	off_t offset = anonymous_buff_alloc_by_offset(buff, newsize);
	return (offset >= 0) ? ((char *)buff->addr + offset)  : NULL;
}



void
anonymous_buff_close_file(struct anonymous_buff_t *buff)
{
	struct buff_list_t *itr, *tmp;
	list_for_each_safe(itr, tmp, &buff->head, node) {
		list_remove(&itr->node);
		free(itr);
	}
	munmap(buff->addr, buff->size);
	close(buff->fd);
	buff->fd = -1;
	buff->size = 0;
}
