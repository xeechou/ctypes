#include <stdlib.h>
#include <unistd.h>
#include "exec.h"

void fork_exec(int argc, char *argv[])
{
	pid_t pid = fork();
	if (pid == -1) {

	} else if (pid == 0) {
		//you don't need to setsid since you want to close all the
		//clients as we close, also, child program can close the stdin,
		//stdout, stderr themselves.

		//child process, we should close all the
		//setsid, we should probably setsid, close stdin, stdout
		//what we can do here is changing the envals.
		execv(argv[0], &argv[1]);
	} else {
		return;
	}
}
