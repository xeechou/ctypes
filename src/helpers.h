#ifndef CTYPES_HELPERS_H
#define CTYPES_HELPERS_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stddef.h>


#ifdef USE_PAGE
#define xmalloc(x) page_malloc(x)
#define xfree(x) page_free(x)
#else
#define xmalloc(x) malloc(x)
#define xfree(x) free(x)
#endif

void dummy_free(void *addr);


typedef int (*VecCmpF) (const void *elemAddr1,
			const void *elemAddr2);
typedef void (*freefun) (void *elemAddr);
typedef void (*VecMapF) (void *elemAddr, void *auxData);

#ifndef deref
#define deref(type, ptr)		*(type)(ptr)
#endif

#ifndef container_of
#define container_of(ptr, type, member)					\
			(type *)(  (char *)(ptr) - offsetof(type, member)   )
#endif

#ifndef container_of_sm
#define container_of_sm(ptr, sample, member)				\
	(__typeof__(sample))((char *)(ptr) -				\
			     offsetof(__typeof__(*sample), member))
#endif


#ifdef __cplusplus
}
#endif

#endif /* EOF */
