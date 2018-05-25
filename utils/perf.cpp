
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <gmapping/utils/perf.h>

#ifdef TRACE_ENABLE

namespace GMapping {

#define MAX_MSG_LENGTH 256

static int fd = -1;

static void __attribute__((constructor)) trace_setup() {
  fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY);
}

TraceScope::TraceScope(const char * str) {
  char buf[MAX_MSG_LENGTH];
  size_t len;
  len = snprintf(buf, MAX_MSG_LENGTH, "B|%d|%s", getpid(), str);
  write(fd, buf, len);
}

TraceScope::~TraceScope() {
  char c = 'E';
  write(fd, &c, sizeof(c));
}

}

#endif
