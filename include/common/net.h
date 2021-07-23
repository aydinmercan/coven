#pragma once

#include <stddef.h>
#include <stdint.h>

int bind_unix_socketfd(const char *path) __attribute__((access(read_only, 1), nonnull));
