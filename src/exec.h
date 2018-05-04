#ifndef CTYPES_EXEC_H
#define CTYPES_EXEC_H

#include <unistd.h>


#ifdef __cplusplus
#extern "C" {
#endif


void fork_exec(int argc, char *argv[]);



#ifdef __cplusplus
}
#endif



#endif /* EOF */
