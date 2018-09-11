#include <stdio.h>
#include <time.h>

// hook is a basic hook function taking no arguments.
void hook(void) { printf("Hello from hook!\n"); }

// metric is a hook function taking arguments.
void metric(char *key, double value) { printf("metric %s=%f\n", key, value); }

// timestamp is a very crude timestamp for testing tracing.
static double timestamp() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec + ((double)t.tv_nsec * 1e-9);
}

// trace_enter called on function entry
void trace_enter(char *name) {
  printf("\nTRACE: enter %s %f\n", name, timestamp());
}

// trace_leave called on function exit
void trace_leave(char *name) {
  printf("\nTRACE: leave %s %f\n", name, timestamp());
}
