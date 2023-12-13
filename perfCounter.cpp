#include "perfCounter.h"

using namespace perf;

// perf counter syscall
int perf_event_open(struct perf_event_attr *hw, pid_t pid, int cpu, int grp,
                    unsigned long flags) {
  return syscall(__NR_perf_event_open, hw, pid, cpu, grp, flags);
}

void PerfCounter::open(const pid_t pid, const int cpu, const int grp,
                       const unsigned long flags) {
  asm volatile("nop;"); // pseudo-barrier
  fd = syscall(__NR_perf_event_open, &attr, pid, cpu, grp, flags);
  asm volatile("nop;"); // pseudo-barrier
  if (fd < 0)
    throw std::runtime_error("Error: Open PerfData::open");
}

void PerfCounter::open() { this->open(getpid(), -1, -1, 0); }
