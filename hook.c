#include <stdio.h>

// hook is a basic hook function taking no arguments.
void hook(void) { printf("Hello from hook!\n"); }

// metric is a hook function taking arguments.
void metric(char *key, double value) { printf("metric %s=%f\n", key, value); }

// trace_enter called on function entry
void trace_enter(char *name) { printf("enter %s {\n", name); }

// trace_leave called on function exit
void trace_leave(char *name) { printf("} // leave %s\n", name); }
