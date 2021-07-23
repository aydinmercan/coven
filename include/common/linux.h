#pragma once

#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

struct clone_args;

pid_t clone3(struct clone_args *cl_args, size_t size);

int pidfd_send_signal(int pidfd, int sig, siginfo_t *info, unsigned int flags);
