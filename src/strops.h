#ifndef CTYPES_STROPS_H
#define CTYPES_STROPS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief zero terminated strncpy
 */
static inline char *
strop_ncpy(char *dest, const char *src, size_t size)
{
	if (size == 0)
		return NULL;
	strncpy(dest, src, size-1);
	dest[size-1] = '\0';

	return dest;
}

static inline char *
strop_ltrim(char *line)
{
	char *ptr = line;
	while (*ptr && isspace(*ptr)) ptr++;
	return ptr;
}

static inline void
strop_rtrim(char *line)
{
	char *ptr = line + strlen(line) - 1;
	while(ptr >= line && *ptr && isspace(*ptr)) *ptr-- = '\0';
}

#ifdef __cplusplus
}
#endif

#endif /* EOF */
