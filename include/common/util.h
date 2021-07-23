#pragma once

#include <signal.h>
#include <stddef.h>
#include <stdint.h>

int block_common_signals(sigset_t *const mask) __attribute__((access(write_only, 1), nonnull));
int unblock_common_signals(void);
