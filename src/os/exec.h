#ifndef CTYPES_EXEC_H
#define CTYPES_EXEC_H

#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif


int fork_exec(int argc, char *const argv[]);



#ifdef __cplusplus
}
#endif



#endif /* EOF */
