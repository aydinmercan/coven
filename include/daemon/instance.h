#pragma once

#include <signal.h>
#include <stddef.h>
#include <stdint.h>

struct daemon_instance {
	uint64_t id;
	int pidfd;
};

struct daemon_instance_map {
	struct daemon_instance instance[32];
};

int daemon_instance_signal(struct daemon_instance *ins, const int sig, siginfo_t *const info)
    __attribute__((nonnull));
