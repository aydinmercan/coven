#include <sys/socket.h>

#include "daemon/event.h"

void daemon_event_ipc_accept_handle(struct daemon_supervisor *const sv)
{
	int fd = accept(sv->unix_fd, NULL, 0U);

	if (fd == -1) {
		return;
	}

	sv->ipc_conn_len++;

	if (sv->ipc_conn_len == 8) {
		return;
	}

	if (daemon_supervisor_poll_ipc_accept(sv) != 0) {
		return;
	}
}
