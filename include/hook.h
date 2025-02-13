#pragma once

#include <cstdio>

int install_hooks();
static ssize_t (*tick_func)();
static ssize_t tick_hook();
