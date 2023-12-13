#include "perfCounter.h"

using namespace perf;

// perf counter syscall
int perf_event_open(struct perf_event_attr *hw, pid_t pid, int cpu, int grp,
                    unsigned long flags) {
  return syscall(__NR_perf_event_open, hw, pid, cpu, grp, flags);
}
