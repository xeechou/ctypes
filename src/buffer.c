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

#include "list.h"
#include "buffer.h"

static int
set_cloexec_or_close(int fd)
{
	long flags;

	if (fd == -1)
		return -1;

	flags = fcntl(fd, F_GETFD);
	if (flags == -1)
		goto err;

	if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
		goto err;

	return fd;

err:
	close(fd);
	return -1;
}

static int
create_tmpfile_cloexec(char *tmpname)
{
	int fd;

#ifdef HAVE_MKOSTEMP
	fd = mkostemp(tmpname, O_CLOEXEC);
	if (fd >= 0)
		unlink(tmpname);
#else
	fd = mkstemp(tmpname);
	if (fd >= 0) {
		fd = set_cloexec_or_close(fd);
		unlink(tmpname);
	}
#endif
	return fd;
}

static int
os_create_anonymous_file(off_t size)
{
	static const char template[] = "/Anonymous-XXXXXX";
	const char *path;
	char *name;
	int fd;

	path = getenv("XDG_RUNTIME_DIR");
	if (!path) {
		errno = ENOENT;
		return -1;
	}

	name = malloc(strlen(path) + sizeof(template));
	if (!name)
		return -1;
	strcpy(name, path);
	strcat(name, template);
	fd = create_tmpfile_cloexec(name);
	free(name);

	if (fd < 0)
		return -1;
	if (ftruncate(fd, size) < 0) {
		close(fd);
		return -1;
	}
	return fd;
}

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
