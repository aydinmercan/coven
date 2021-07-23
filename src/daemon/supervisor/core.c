#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/signalfd.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common/net.h"
#include "common/util.h"
#include "daemon/event.h"
#include "daemon/log.h"
#include "daemon/supervisor.h"

static inline void __attribute__((access(read_only, 1), always_inline))
unwatch_and_close_fd(const struct daemon_supervisor *const sv, const int fd)
{
	if (epoll_ctl(sv->epoll_fd, EPOLL_CTL_DEL, fd, NULL) != 0) {
		daemon_log_fail("epoll_ctl_del fd %d - %s", fd, strerror(errno));
	}

	if (close(fd) != 0) {
		daemon_log_fail("close fd %d - %s", fd, strerror(errno));
	}
}

// don't allocate here, ever
static inline int __attribute__((access(write_only, 1), always_inline))
determine_unix_ipc_path(struct daemon_supervisor *const sv)
{
	// Root?
	if (getuid() == 0) {
		sv->unix_path = (char *) "/run";
		return 0;
	}

	// XDG Runtime Directory?
	char *xdg = getenv("XDG_RUNTIME_DIR");
	if (xdg) {
		sv->unix_path = xdg;
		return 0;
	}

	return -1;
}

static inline int __attribute__((access(read_write, 1), always_inline))
create_and_watch_unix_socketfd(struct daemon_supervisor *const sv)
{
	if (determine_unix_ipc_path(sv) != 0) {
		goto socket_dir_FAULT;
	}

	if (chdir(sv->unix_path) != 0) {
		goto socket_dir_FAULT;
	}

	int fd = bind_unix_socketfd("coven.sock");
	if (fd == -1) {
		goto socket_FAULT;
	}

	if (listen(fd, SOMAXCONN) != 0) {
		goto listen_FAULT;
	}

	sv->unix_fd = fd;

	if (daemon_supervisor_watch_event_ipc_accept(sv) != 0) {
		goto watch_FAULT;
	}

	return 0;

watch_FAULT:
listen_FAULT:
	unlink("coven.sock");
	close(fd);
socket_FAULT:
socket_dir_FAULT:
	return -1;
}

static inline int __attribute__((always_inline))
create_and_watch_signalfd(struct daemon_supervisor *const sv)
{
	sigset_t mask;

	if (block_common_signals(&mask) != 0) {
		return -1;
	}

	int fd = signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC);
	if (fd == -1) {
		return -1;
	}

	sv->signal_fd = fd;
	if (daemon_supervisor_watch_event_signalfd(sv) != 0) {
		close(fd);
		return -1;
	}

	return 0;
}

static inline void __attribute__((always_inline))
cleanup_signal_fd_and_event(struct daemon_supervisor *const sv)
{
	unwatch_and_close_fd(sv, sv->signal_fd);
}

static inline void __attribute__((always_inline))
cleanup_unix_fd_and_event(struct daemon_supervisor *const sv)
{
	if (chdir(sv->unix_path) != 0) {
		daemon_log_fail("chdir to unix_path (%s) failed", sv->unix_path);
	} else {
		unlink("coven.sock");
	}

	unwatch_and_close_fd(sv, sv->signal_fd);
}

int daemon_supervisor_init(struct daemon_supervisor *const sv)
{
	size_t i;

	sv->epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	if (sv->epoll_fd == -1) {
		goto epoll_FAULT;
	}

	for (i = 0; i < 128; i++) {
		sv->events[i].type = DAEMON_EVENT_NONE;
	}

	if (create_and_watch_signalfd(sv) != 0) {
		goto signal_FAULT;
	}

	if (create_and_watch_unix_socketfd(sv) != 0) {
		return -1;
	}

	sv->running      = 1;
	sv->instance_len = 0U;

	DAEMON_DEBUG_DESERIALIZE(sv);

	return 0;

signal_FAULT:
	(void) close(sv->epoll_fd);
epoll_FAULT:
	return -1;
}

void daemon_supervisor_deinit(struct daemon_supervisor *const sv)
{
	int fd;
	size_t i;

	for (i = 0; i < 128; i++) {
		switch (sv->events[i].type) {
		case DAEMON_EVENT_SERVICE:
			break;
		case DAEMON_EVENT_SIGNAL:
			cleanup_signal_fd_and_event(sv);
			break;
		case DAEMON_EVENT_IPC_ACCEPT:
			cleanup_unix_fd_and_event(sv);
			break;
		case DAEMON_EVENT_IPC_CONN:
			fd = sv->events[i].ipc.fd;
			shutdown(fd, SHUT_RDWR);
			unwatch_and_close_fd(sv, fd);
			break;
		}
	}

	(void) close(sv->epoll_fd);
}
