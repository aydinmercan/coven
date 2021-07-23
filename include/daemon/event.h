#pragma once

#include <stddef.h>
#include <stdint.h>

#define DAEMON_EVENT_NONE       (0x00)
#define DAEMON_EVENT_SERVICE    (0x01)
#define DAEMON_EVENT_SIGNAL     (0x02)
#define DAEMON_EVENT_IPC_ACCEPT (0x03)
#define DAEMON_EVENT_IPC_CONN   (0x04)

// Events are essentially tagged unions

struct daemon_supervisor;

struct daemon_event_service {
	struct daemon_instance *instance;
};

struct daemon_event_ipc {
	int fd;
};

struct daemon_event {
	uint_fast8_t type;
	union {
		struct daemon_event_service service;
		struct daemon_event_ipc ipc;
	};
};

void daemon_event_signal_handle(struct daemon_supervisor *const sv) __attribute__((nonnull));
void daemon_event_ipc_accept_handle(struct daemon_supervisor *const sv) __attribute__((nonnull));
void daemon_event_ipc_hangup_handle(struct daemon_supervisor *const sv) __attribute__((nonnull));
