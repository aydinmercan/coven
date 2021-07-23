#include <inttypes.h>
#include <sys/epoll.h>

#include "daemon/event.h"
#include "daemon/log.h"
#include "daemon/supervisor.h"

int daemon_supervisor_get_free_event_index(struct daemon_supervisor *const sv,
                                           uint64_t *const index)
{
	uint64_t i;

	for (i = 0; i < 128; i++) {
		if (sv->events[i].type == DAEMON_EVENT_NONE) {
			*index = i;
			return 0;
		}
	}

	return -1;
}

int daemon_supervisor_watch_event_signalfd(struct daemon_supervisor *const sv)
{
	uint64_t i;

	if (daemon_supervisor_get_free_event_index(sv, &i) != 0) {
		return -1;
	}

	DAEMON_DEBUG("signal event at index %" PRIu64, i);

	sv->events[i].type = DAEMON_EVENT_SIGNAL;

	struct epoll_event ev = {
	    .events   = EPOLLIN | EPOLLEXCLUSIVE,
	    .data.u64 = i,
	};

	if (epoll_ctl(sv->epoll_fd, EPOLL_CTL_ADD, sv->signal_fd, &ev) != 0) {
		return -1;
	}

	return 0;
}

int daemon_supervisor_watch_event_ipc_accept(struct daemon_supervisor *const sv)
{
	uint64_t i;

	if (daemon_supervisor_get_free_event_index(sv, &i) != 0) {
		return -1;
	}

	DAEMON_DEBUG("IPC accept at index %" PRIu64, i);

	sv->events[i].type = DAEMON_EVENT_IPC_ACCEPT;

	struct epoll_event ev = {
	    .events   = EPOLLIN | EPOLLEXCLUSIVE,
	    .data.u64 = i,
	};

	if (epoll_ctl(sv->epoll_fd, EPOLL_CTL_ADD, sv->unix_fd, &ev) != 0) {
		return -1;
	}

	return 0;
}

int daemon_supervisor_watch_event_ipc_connection(struct daemon_supervisor *const sv, const int fd)
{
	uint64_t i;

	if (daemon_supervisor_get_free_event_index(sv, &i) != 0) {
		return -1;
	}

	sv->events[i].type   = DAEMON_EVENT_IPC_CONN;
	sv->events[i].ipc.fd = fd;

	struct epoll_event ev = {
	    .events   = EPOLLIN | EPOLLHUP | EPOLLEXCLUSIVE,
	    .data.u64 = i,
	};

	if (epoll_ctl(sv->epoll_fd, EPOLL_CTL_ADD, fd, &ev) != 0) {
		return -1;
	}

	return 0;
}
