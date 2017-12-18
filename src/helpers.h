#ifndef CTYPES_HELPERS_H
#define CTYPES_HELPERS_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>


#ifdef USE_PAGE
#define xmalloc(x) page_malloc(x)
#define xfree(x) page_free(x)
#else
#define xmalloc(x) malloc(x)
#define xfree(x) free(x)
#endif

static void dummy_free(void *addr) {}


typedef int (*VecCmpF) (const void *elemAddr1,
			const void *elemAddr2);
typedef void (*freefun) (void *elemAddr);
typedef void (*VecMapF) (void *elemAddr, void *auxData);

#define ptr_derefer(type, ptr)	((type) *)(ptr)


#ifdef __cplusplus
}
#endif

#endif /* EOF */