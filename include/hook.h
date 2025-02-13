#pragma once

#include <cstdio>

int install_hooks(ssize_t startingAddress);
static ssize_t (*tick_func)(long);
static ssize_t tick_hook(long);
