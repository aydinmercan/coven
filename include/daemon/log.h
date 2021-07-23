#pragma once

#include <stddef.h>
#include <stdint.h>

struct daemon_event;
struct daemon_supervisor;
struct signalfd_siginfo;

#ifndef NDEBUG
#define DAEMON_DEBUG(...) daemon_log_debug(__VA_ARGS__)

// clang-format off
#define DAEMON_DEBUG_DESERIALIZE(X)                                             \
	_Generic((X),                                                           \
		struct daemon_supervisor * : daemon_log_deserialize_supervisor, \
		struct daemon_event * : daemon_log_deserialize_event,           \
		struct signalfd_siginfo * : daemon_log_deserialize_signalfd     \
	)(X)
// clang-format on
#else
#define DAEMON_DEBUG(...)
#define DAEMON_DEBUG_DESERIALIZE(X)
#endif

int daemon_log_init(void);
void daemon_log_ok(const char *restrict fmt, ...);
void daemon_log_info(const char *restrict fmt, ...);
void daemon_log_fail(const char *restrict fmt, ...);

#ifndef NDEBUG
void daemon_log_debug(const char *restrict fmt, ...) __attribute__((nonnull));

void daemon_log_deserialize_signalfd(const struct signalfd_siginfo *const info)
    __attribute__((access(read_only, 1), nonnull));

void daemon_log_deserialize_supervisor(const struct daemon_supervisor *const sv)
    __attribute__((access(read_only, 1), nonnull));

void daemon_log_deserialize_event(const struct daemon_event *const ev)
    __attribute__((access(read_only, 1), nonnull));
#endif
