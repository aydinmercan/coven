#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "daemon/log.h"
#include "daemon/supervisor.h"

static _Noreturn void usage(const int status)
{
	// clang-format off
	printf(
		"covend: naive service supervisor\n"
		"Usage:\n"
		"  covend [SVDIR]\n"
		"  covend [--help]\n"
		"Options:\n"
		"  -h, --help: display this screen and exit\n"
		"\n"
		"SVDIR is the folder with the compiled service entries\n"
	);
	// clang-format on
	exit(status);
}

int main(int argc, char **argv)
{
	struct daemon_supervisor sv;
	int opt, opt_idx;

	static struct option opts[] = {
	    {"help", no_argument, 0, 'h'},
	    {0, 0, 0, 0},
	};

	while ((opt = getopt_long(argc, argv, "h", opts, &opt_idx)) != -1) {
		switch (opt) {
		case 'h':
			usage(EXIT_SUCCESS);
		case '?':
			__attribute__((fallthrough));
		default:
			usage(EXIT_FAILURE);
		}
	}

	if (daemon_log_init() != 0) {
		exit(EXIT_FAILURE);
	}

	if (daemon_supervisor_init(&sv) != 0) {
		daemon_log_fail("supervisor initialize");
		exit(EXIT_FAILURE);
	}

	while (sv.running == 1) {
		daemon_supervisor_loop(&sv);
	}

	daemon_supervisor_deinit(&sv);

	return 0;
}
