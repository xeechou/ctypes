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

#ifndef CTYPES_HELPERS_H
#define CTYPES_HELPERS_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

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

#ifndef instance_of
#define instance_of(type)   ({ (type *)(0); })
#endif

#ifndef __type_assert__
#define __type_assert__(ta, tb)			\
	({_Static_assert( __builtin_types_compatible_p(ta, tb), "type not equal"); })
#endif

#ifndef __member_of__
#define __member_of__(type, mem)	\
	({ &((type *)0)->mem; })
#endif

/* how do we make a safer container of */
#ifndef container_of
#define container_of(ptr, type, member)					\
	({  __type_assert__( __typeof__(ptr),				\
			     __typeof__(&instance_of(type)->member));	\
		(type *)(  (char *)(ptr) - offsetof(type, member) ); })
#endif

#ifndef container_of_sm
#define container_of_sm(ptr, sample, member)				\
	(__typeof__(sample))((char *)(ptr) -				\
			     offsetof(__typeof__(*sample), member))
#endif

#if defined(__GNUC__) && __GNUC__ >= 4
#define UNUSED_ARG(arg) __attribute__((unused)) arg
#elif defined(__clang__) && __clang__ >= 3
#define UNUSED_ARG(arg) __attribute__((unused)) arg
#else
#define UNUSED_ARG(arg) arg
#endif

#ifndef MAX
#define MAX(a, b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })
#endif

#ifndef MIN
#define MIN(a, b) \
	({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; })
#endif

#ifndef INBOUND
#define INBOUND(x, l, r) \
	({ ( (x) > (l) && (x) <= (r) ) ? true : false; })
#endif

#ifndef ABS
#define ABS(x) \
	({ __typeof__ (x) _x = (x); \
		_x < 0 ? -_x : _x; })
#endif

#ifndef CLAMP
#define CLAMP(x, a, b) \
	({ __typeof__ (x) _x = (x); \
		MIN(MAX(_x, (a)), (b));})
#endif

/* (1-r) * a + r * b */
#ifndef MIX
#define MIX(r, a, b) \
	({ __typeof__ (r) _r = CLAMP(r, 0.0, 1.0); \
		((1.0 - _r) * (a)) + (_r * (b)); })
#endif

/* remember typeof gets replaced to the exact type, so we need to casting */
#ifndef STEP
#define STEP(x, e)  \
	({ (x) < (e) ? (__typeof__ (x)) 0 : (__typeof__ (x)) 1;  })
#endif

#ifndef SMOOTHSTEP
#define SMOOTHSTEP(x, e0, e1) \
	({ __typeof__ (x) _t = CLAMP((x - e0) / (e1 - e0), 0.0, 1.0); \
		_t * _t * (3.0 - 2.0 * _t);})
#endif


#ifndef NUMOF
#define NUMOF(x) (sizeof (x) / sizeof (*x) )
#endif

/* /\* from this, you can acess a memeber of a struct with its offset *\/ */
/* #ifndef PTRMEMB */
/* #define PTRMEMB(ptr, offset, type) (type *)((char *)(ptr) + offset) */
/* #endif */

#ifndef SWAP
#define SWAP(a, b)		      \
	({ __typeof__ (a) _tmp = (a); \
		a = (b);	      \
		b = (_tmp);})
#endif

#ifndef OPTION
/* option directive give us ability to define types like std::option */
#define OPTION(type, name) \
	struct tw_option_ ## name { bool valid; type name; }
#endif

/* get rid of annoying assert unsued variables */
#ifdef NDEBUG
#define ASSERT(x) do { if(!(x));} while (0)
#else
#define ASSERT(x) assert(x)
#endif

#ifdef __cplusplus
}
#endif

#endif /* EOF */
