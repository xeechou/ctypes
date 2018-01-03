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
anonymous_buff_new(struct anonymous_buff_t *buff, off_t size)
{
	buff->fd = os_create_anonymous_file(size);
	list_init(&buff->head);
	buff->size = size;
	return buff->fd;
}


void *
anonymous_buff_alloc(struct anonymous_buff_t *buff, off_t newsize, int prot, int flags)
{
	struct buff_list_t *itr;
	off_t allocated = 0;
	list_for_each(itr, &buff->head, node) {
		allocated += itr->size;
	}
	if ((int)buff->size - (int)allocated  < newsize)
		return NULL;
	//otherwise, we can do the insert
	itr = malloc(sizeof(*itr));
	itr->size = newsize;
	itr->data = mmap(NULL, newsize, prot, flags, buff->fd, allocated);
	if (itr->data == MAP_FAILED) {
		free(itr);
		return NULL;
	}
	list_append(&buff->head, &itr->node);
	return itr->data;
}

void
anonymous_buff_close_file(struct anonymous_buff_t *buff)
{
	struct buff_list_t *itr, *tmp;
	list_for_each_safe(itr, tmp, &buff->head, node) {
		list_remove(&itr->node);
		munmap(itr->data, itr->size);
		free(itr);
	}
	close(buff->fd);
	buff->fd = -1;
	buff->size = 0;
}
