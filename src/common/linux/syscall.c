#include <linux/sched.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "common/linux.h"

pid_t clone3(struct clone_args *cl_args, size_t size)
{
	return (pid_t) syscall(SYS_clone3, cl_args, size);
}

int pidfd_send_signal(int pidfd, int sig, siginfo_t *info, unsigned int flags)
{
	return (int) syscall(SYS_pidfd_send_signal, pidfd, sig, info, flags);
}
