#ifndef CTYPES_HELPERS_H
#define CTYPES_HELPERS_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stddef.h>

/* you may want to use stb_memcheck for the leak checking purpose */

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


#ifndef max
#define max(a, b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })
#endif

#ifndef min
#define min(a, b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; })
#endif

/* TODO test this ! */
#ifndef abs
#define abs(x) \
	({ __typeof__ (x) _x = (x); \
		_x < 0 ? -_x : _x; })
#endif

/* TODO test this ! */
#ifndef clamp
#define clamp(x, a, b) \
	({ __typeof__ (x) _x = (x); \
		min(max(_x, (a)), (b));})
#endif

/* TODO test this ! */
#ifndef step
#define step(x, e)  \
	({ (x) < e ? __typeof__(x) 0 : __typeof__(x) 1;  })
#endif

/* TODO test this ! */
// #ifndef smoothstep
// #define smoothstep(x, a, b) \
//	({})
// #endif

#ifndef numof
#define numof(x) (sizeof (x) / sizeof (*x) )
#endif

/* from this, you can acess a memeber of a struct with its offset */
#ifndef ptrmemb
#define ptrmemb(ptr, offset, type) (type *)((char *)(ptr) + offset)
#endif

#ifndef swap
#define swap(a, b)		      \
	({ __typeof__ (a) _tmp = (a); \
		a = (b);	      \
		b = (_tmp);})
#endif

#ifdef __cplusplus
}
#endif

#endif /* EOF */
