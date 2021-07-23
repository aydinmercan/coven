#include <inttypes.h>
#include <stdio.h>
#include <sys/signalfd.h>
#include <unistd.h>

#include "daemon/event.h"
#include "daemon/log.h"
#include "daemon/supervisor.h"

static inline void __attribute__((always_inline))
log_event_signal(const struct daemon_event *const ev)
{
	// clang-format off
	dprintf(STDERR_FILENO,
		"[ \e[1;35mDBG!\e[0m ] daemon_event\n"
		"{\n"
		"    type : DAEMON_EVENT_SIGNAL (%" PRIuFAST8 ")\n"
		"}\n",
	ev->type
	);
	// clang-format on
}

static inline void __attribute__((always_inline))
log_event_ipc_accept(const struct daemon_event *const ev)
{
	// clang-format off
	dprintf(STDERR_FILENO,
		"[ \e[1;35mDBG!\e[0m ] daemon_event\n"
		"{\n"
		"    type : DAEMON_EVENT_IPC_ACCEPT (%" PRIuFAST8 ")\n"
		"}\n",
	ev->type
	);
	// clang-format on
}

static inline void __attribute__((always_inline))
log_event_ipc_conn(const struct daemon_event *const ev)
{
	// clang-format off
	dprintf(STDERR_FILENO,
		"[ \e[1;35mDBG!\e[0m ] daemon_event\n"
		"{\n"
		"    type : DAEMON_EVENT_SIGNAL (%" PRIuFAST8 ")\n"
		"    fd   : %d\n"
		"}\n",
	ev->type, ev->ipc.fd
	);
	// clang-format on
}

void daemon_log_deserialize_event(const struct daemon_event *const ev)
{
	switch (ev->type) {
	case DAEMON_EVENT_SIGNAL:
		log_event_signal(ev);
		break;
	case DAEMON_EVENT_IPC_ACCEPT:
		log_event_ipc_accept(ev);
		break;
	case DAEMON_EVENT_IPC_CONN:
		log_event_ipc_conn(ev);
		break;
	default:
		daemon_log_fail("DBG! DESERIALIZE UNKNOWN DAEMON_EVENT");
		break;
	}
}

void daemon_log_deserialize_supervisor(const struct daemon_supervisor *const sv)
{
	// clang-format off
	dprintf(STDERR_FILENO,
		"[ \e[1;35mDBG!\e[0m ] daemon_supervisor\n"
		"{\n"
		"    epoll_fd     : %d\n"
		"    signal_fd    : %d\n"
		"    unix_fd      : %d\n"
		"    running      : %u\n"
		"    unix_path    : %s\n"
		"    ipc_conn_len : %zu\n"
		"    instance_len : %zu\n"
		"    instances    : ...\n"
		"}\n",
	sv->epoll_fd, sv->signal_fd, sv->unix_fd, sv->running,
	sv->unix_path, sv->ipc_conn_len, sv->instance_len);
	// clang-format on
}

void daemon_log_deserialize_signalfd(const struct signalfd_siginfo *const info)
{
	// clang-format off
	dprintf(STDERR_FILENO,
		"[ \e[1;35mDBG!\e[0m ] signalfd_siginfo\n"
		"{\n"
		"    ssi_signo : %" PRIu32"\n"
		"}\n",
	info->ssi_signo);
	// clang-format on
}
