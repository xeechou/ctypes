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

#ifndef CTYPE_MAGIC_HPP
#define CTYPE_MAGIC_HPP


#ifdef __cplusplus
#extern "C" {
#endif

static inline int
abs(int a)
{
	int s = a >> sizeof(int) * 8 - 1;	//s == -1 if a < 0, else 0
	a ^= s;					//any number ^ -1 is one's complement
	a -= s;					//a - (-1) if a is one's complement
	return a;
}

/* return a - (a - b) if a < b, else return a - 0 */
static inline int
max(int a, int b)
{
	int diff = a - b;
	int dsign = diff >> sizeof(int) * 8 - 1;
	return a - (diff & dsign);
}

/* return b + (a - b) if a < b, else return b + 0 */
static inline int
min(int a, int b)
{
	int diff = a - b;
	int dsign = diff >> sizeof(int) * 8 - 1;
	return b + (diff & dsign);
}

#define _max((a), (b)) ((a) > (b)) ? (a) : (b)
#define _min((a), (b)) ((a) > (b)) ? (b) : (a)

/* conditional increment */

#define _conditional_incre((x), (cond), (inc))	(x) += -(cond)  & (inc)

#ifdef __cplusplus
}
#endif


/* increase pointer if condition */
/* if (a > b)
 *	*ptr++ = value
 * now becomes
 *
 * *ptr = value
 * ptr += (a > b)
 */

#endif /* EOF */
