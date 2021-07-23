#pragma once

#include <stddef.h>
#include <stdint.h>

#include "daemon/event.h"

#define DAEMON_SUPERVISOR_EVENT_LOOP_SIZE (32)

struct daemon_supervisor {
	int epoll_fd;
	int signal_fd;
	int unix_fd;
	_Bool running;
	char *unix_path;
	size_t ipc_conn_len;
	size_t instance_len;
	struct daemon_event events[128];
};

int daemon_supervisor_init(struct daemon_supervisor *const sv) __attribute__((nonnull));
void daemon_supervisor_deinit(struct daemon_supervisor *const sv) __attribute__((nonnull));

int daemon_supervisor_get_free_event_index(struct daemon_supervisor *const sv,
                                           uint64_t *const index)
    __attribute__((access(write_only, 2), nonnull));

void daemon_supervisor_loop(struct daemon_supervisor *const sv)
    __attribute__((access(read_write, 1), nonnull));

int daemon_supervisor_watch_event_signalfd(struct daemon_supervisor *const sv)
    __attribute__((nonnull));

int daemon_supervisor_watch_event_ipc_accept(struct daemon_supervisor *const sv)
    __attribute__((nonnull));

int daemon_supervisor_watch_event_ipc_connection(struct daemon_supervisor *const sv, const int fd)
    __attribute__((nonnull));
