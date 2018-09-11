#include <stdio.h>

// hook is a basic hook function taking no arguments.
void hook(void) { printf("Hello from hook!\n"); }

// metric is a hook function taking arguments.
void metric(char *key, double value) { printf("metric %s=%f\n", key, value); }