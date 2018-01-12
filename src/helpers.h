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
#define xrealloc(x) page_realloc(x)
#else
#define xmalloc(x) malloc(x)
#define xfree(x) free(x)
#define xrealloc(x) realloc(x)
#define xcalloc(n,x) calloc(x,n)
#endif

typedef int (*cmpfun) (const void *elemAddr1,
			const void *elemAddr2);
typedef void (*freefun) (void *elemAddr);
typedef void (*VecMapF) (void *elemAddr, void *auxData);
void dummy_free(void *addr);


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


#ifndef num
#define num(x) (sizeof (x) / sizeof (*x) )
#endif

/* from this, you can acess a memeber of a struct with its offset */
#ifndef ptrmemb
#define ptrmemb(ptr, offset, type) (type *)((char *)(ptr) + offset)
#endif


#ifdef __cplusplus
}
#endif

#endif /* EOF */
