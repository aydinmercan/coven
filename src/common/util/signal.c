#include <signal.h>
#include <unistd.h>

#include "common/util.h"

static inline int __attribute__((access(write_only, 1), always_inline, nonnull))
sigset_populate_common_signals(sigset_t *const mask)
{
	if (sigemptyset(mask) != 0) {
		return -1;
	}

	if (sigaddset(mask, SIGTERM) != 0) {
		return -1;
	}

	if (sigaddset(mask, SIGINT) != 0) {
		return -1;
	}

	if (sigaddset(mask, SIGCHLD) != 0) {
		return -1;
	}

	if (sigaddset(mask, SIGHUP) != 0) {
		return -1;
	}

	return 0;
}

int block_common_signals(sigset_t *const mask)
{
	if (sigset_populate_common_signals(mask) != 0) {
		return -1;
	}

	return pthread_sigmask(SIG_BLOCK, mask, NULL);
}

int unblock_common_signals(void)
{
	sigset_t mask;

	if (sigset_populate_common_signals(&mask) != 0) {
		return -1;
	}

	return pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
}
