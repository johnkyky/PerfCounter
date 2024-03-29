#pragma once

#include <cstdint>
#include <linux/perf_event.h>
#include <linux/unistd.h>
#include <stdint.h>
#include <syscall.h>
#include <unistd.h>

namespace perf {

enum HARDWARE_EVENT_TYPE {
  NB_CYCLES = PERF_COUNT_HW_CPU_CYCLES,
  NB_INSTRUCTIONS = PERF_COUNT_HW_INSTRUCTIONS,
  CACHE_REFERENCES = PERF_COUNT_HW_CACHE_REFERENCES,
  CACHE_MISSES = PERF_COUNT_HW_CACHE_MISSES,
  BRANCH_INSTRUCTIONS = PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
  BRANCH_MISSES = PERF_COUNT_HW_BRANCH_MISSES
};

enum SOFTWARE_EVENT_TYPE {
  CPU_CLOCK = PERF_COUNT_SW_CPU_CLOCK, // nano seconds
  TASK_CLOCK = PERF_COUNT_SW_TASK_CLOCK,
  PAGE_FAULTS = PERF_COUNT_SW_PAGE_FAULTS,
  CONTEXT_SWITCHES = PERF_COUNT_SW_CONTEXT_SWITCHES,
  CPU_MIGRATIONS = PERF_COUNT_SW_CPU_MIGRATIONS,
};

class PerfCounters {
private:
  struct perf_event_attr attr;
  uint64_t val_begin = 0;
  uint64_t val_end = 0;
  int fd;
  int rc;
  bool counting;

public:
  PerfCounters() = delete;

  PerfCounters(const struct perf_event_attr &attr);

  PerfCounters(const struct perf_event_attr &&attr);

  PerfCounters(const unsigned int type, const unsigned long long config,
               const unsigned long long disabled);

  PerfCounters(const HARDWARE_EVENT_TYPE type);

  PerfCounters(const SOFTWARE_EVENT_TYPE type);

  ~PerfCounters();

  void open(const pid_t pid, const int cpu, const int grp,
            const unsigned long flags);

  void open(const pid_t pid);

  void open();

  void start();

  void stop();

  uint64_t getValue();
};

} // namespace perf
