#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <sys/signalfd.h>
#include <unistd.h>

#include "daemon/event.h"
#include "daemon/log.h"
#include "daemon/supervisor.h"

void daemon_event_signal_handle(struct daemon_supervisor *const sv)
{
	struct signalfd_siginfo info;

	if (read(sv->signal_fd, &info, sizeof(info)) != sizeof(struct signalfd_siginfo)) {
		daemon_log_fail("failed to read signalfd information");
		sv->running = 0;
	}

	DAEMON_DEBUG_DESERIALIZE(&info);

	switch (info.ssi_signo) {
	case SIGCHLD:
		daemon_log_info("SIGCHLD from PID %" PRIu32 " currently ignoring...", info.ssi_pid);
		break;
	case SIGHUP:
		daemon_log_info("SIGHUP from PID %" PRIu32 " currently ignoring...", info.ssi_pid);
		break;
	case SIGTERM:
		__attribute__((fallthrough));
	case SIGINT:
		DAEMON_DEBUG("signal to stop recieved...");
		sv->running = 0;
		break;
	default:
		daemon_log_info("unhandled signal %d", info.ssi_signo);
	}
}
