#include "common/linux.h"
#include "daemon/instance.h"

int daemon_instance_signal(struct daemon_instance *ins, const int sig, siginfo_t *const info)
{
	return pidfd_send_signal(ins->pidfd, sig, info, 0x00);
}
