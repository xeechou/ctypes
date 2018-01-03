#ifndef CTYPES_BUFF_H
#define CTYPES_BUFF_H

#include "list.h"

#ifdef __cplusplus
#extern "C" {
#endif

struct buff_list_t {
	list_t node;
	size_t size;
	void *data;
};

struct anonymous_buff_t {
	int fd;
	int size;
	list_t head;
};


int anonymous_buff_new(struct anonymous_buff_t *buff, off_t size);

void *anonymous_buff_alloc(struct anonymous_buff_t *buff, off_t newsize, int prot, int flags);

void anonymous_buff_close_file(struct anonymous_buff_t *buff);


#ifdef __cplusplus
}
#endif


#endif
