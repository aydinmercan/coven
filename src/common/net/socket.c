#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "common/net.h"

// The function is too small to deduplicate errors
int bind_unix_socketfd(const char *path)
{
	int fd;
	struct sockaddr_un un = {
	    .sun_family = AF_UNIX,
	    .sun_path   = {0},
	};

	// Ensure last index is always reserved for NUL termination
	(void) strncpy(un.sun_path, path, 107);

	fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0x00);
	if (fd == -1) {
		return -1;
	}

	if (bind(fd, (struct sockaddr *) &un, sizeof(struct sockaddr_un)) != 0) {
		return -1;
	}

	return fd;
}
