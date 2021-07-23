#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include "daemon/event.h"
#include "daemon/log.h"
#include "daemon/supervisor.h"

void daemon_supervisor_loop(struct daemon_supervisor *const sv)
{
	struct epoll_event polled[DAEMON_SUPERVISOR_EVENT_LOOP_SIZE];
	uint64_t index;
	int i, nlen;

	nlen = epoll_wait(sv->epoll_fd, polled, DAEMON_SUPERVISOR_EVENT_LOOP_SIZE, -1);
	if (nlen == -1) {
		daemon_log_fail("epoll");
		sv->running = 0;
	}

	DAEMON_DEBUG("event loop %d", nlen);

	for (i = 0; i < nlen; i++) {
		index = polled[i].data.u64;
		DAEMON_DEBUG("event index %" PRIu64, index);

		DAEMON_DEBUG_DESERIALIZE(&sv->events[index]);

		switch (sv->events[index].type) {
		case DAEMON_EVENT_SERVICE:
			DAEMON_DEBUG("Service");
			break;
		case DAEMON_EVENT_SIGNAL:
			daemon_event_signal_handle(sv);
			break;
		case DAEMON_EVENT_IPC_ACCEPT:
			break;
		case DAEMON_EVENT_IPC_CONN:
			break;
		default:
			daemon_log_fail("unknown event type %" PRIuFAST8, sv->events[index].type);
			sv->running = 0;
		}
	}
}
