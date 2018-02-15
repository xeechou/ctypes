#ifndef CTYPES_BUFF_H
#define CTYPES_BUFF_H

#include "list.h"

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
