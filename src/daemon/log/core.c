#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include "daemon/log.h"

// TODO: Make this less stupid

static void (*log_ok_function)(const char *restrict fmt, va_list args)   = NULL;
static void (*log_info_function)(const char *restrict fmt, va_list args) = NULL;
static void (*log_fail_function)(const char *restrict fmt, va_list args) = NULL;

// Colorconsole

static inline void __attribute__((always_inline))
log_common_colorconsole(const char *restrict fmt, va_list args)
{
	(void) vdprintf(STDERR_FILENO, fmt, args);
}

static void log_ok_colorconsole(const char *restrict fmt, va_list args)
{
	dprintf(STDERR_FILENO, "[  \e[1;32mOK\e[0m  ] ");
	log_common_colorconsole(fmt, args);
	dprintf(STDERR_FILENO, "\n");
}

static void log_info_colorconsole(const char *restrict fmt, va_list args)
{
	dprintf(STDERR_FILENO, "[ \e[1;37mINFO\e[0m ] ");
	log_common_colorconsole(fmt, args);
	dprintf(STDERR_FILENO, "\n");
}

static void log_fail_colorconsole(const char *restrict fmt, va_list args)
{
	dprintf(STDERR_FILENO, "[ \e[1;31mFAIL\e[0m ] ");
	log_common_colorconsole(fmt, args);
	dprintf(STDERR_FILENO, "\n");
}

int daemon_log_init(void)
{
	log_ok_function   = log_ok_colorconsole;
	log_info_function = log_info_colorconsole;
	log_fail_function = log_fail_colorconsole;

	return 0;
}

void daemon_log_ok(const char *restrict fmt, ...)
{
	va_list args;

	va_start(args, fmt);

	log_ok_function(fmt, args);

	va_end(args);
}

void daemon_log_info(const char *restrict fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	log_info_function(fmt, args);
	va_end(args);
}

void daemon_log_fail(const char *restrict fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	log_fail_function(fmt, args);
	va_end(args);
}

#ifndef NDEBUG
void daemon_log_debug(const char *restrict fmt, ...)
{
	va_list args;

	va_start(args, fmt);

	dprintf(STDERR_FILENO, "[ \e[1;35mDBG!\e[0m ] ");
	log_common_colorconsole(fmt, args);
	dprintf(STDERR_FILENO, "\n");

	va_end(args);
}
#endif // NDEBUG
