#pragma once

#include <cstdint>
#include <cstring>
#include <linux/perf_event.h>
#include <linux/unistd.h>
#include <stdexcept>
#include <stdint.h>
#include <syscall.h>
#include <unistd.h>

namespace perf {

// perf counter syscall
int perf_event_open(struct perf_event_attr *hw, pid_t pid, int cpu, int grp,
                    unsigned long flags);

enum HARDWARE_EVENT_TYPE {
  NB_CYCLES = PERF_COUNT_HW_CPU_CYCLES,
  NB_INSTRUCTIONS = PERF_COUNT_HW_INSTRUCTIONS,
  CACHE_REFERENCES = PERF_COUNT_HW_CACHE_REFERENCES,
  CACHE_MISSES = PERF_COUNT_HW_CACHE_MISSES,
  BRANCH_INSTRUCTIONS = PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
  BRANCH_MISSES = PERF_COUNT_HW_BRANCH_MISSES
};

enum SOFTWARE_EVENT_TYPE {
  NB_CYCLES_SW = PERF_COUNT_HW_CPU_CYCLES,
  CPU_CLOCK = PERF_COUNT_SW_CPU_CLOCK,
  TASK_CLOCK = PERF_COUNT_SW_TASK_CLOCK,
  PAGE_FAULTS = PERF_COUNT_SW_PAGE_FAULTS,
  CONTEXT_SWITCHES = PERF_COUNT_SW_CONTEXT_SWITCHES,
  CPU_MIGRATIONS = PERF_COUNT_SW_CPU_MIGRATIONS,
};

class PerfCounter {
private:
  struct perf_event_attr attr;
  uint64_t val_begin = 0;
  uint64_t val_end = 0;
  int fd;
  int rc;
  bool counting;

public:
  PerfCounter() = delete;

  PerfCounter(const struct perf_event_attr &attr) : counting(false) {
    std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
    this->attr.type = attr.type;
    this->attr.config = attr.config;
    this->attr.disabled = attr.disabled;
  }

  PerfCounter(const struct perf_event_attr &&attr) : counting(false) {
    this->attr = std::move(attr);
  }

  PerfCounter(const unsigned int type, const unsigned long long config,
              const unsigned long long disabled)
      : counting(false) {
    std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
    this->attr.type = type;
    this->attr.config = config;
    this->attr.disabled = disabled;
  }

  PerfCounter(const HARDWARE_EVENT_TYPE type) : counting(false) {
    std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
    this->attr.type = PERF_TYPE_HARDWARE;
    this->attr.config = type;
    this->attr.disabled = 0;
  }

  PerfCounter(const SOFTWARE_EVENT_TYPE type) : counting(false) {
    std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
    this->attr.type = PERF_TYPE_SOFTWARE;
    this->attr.config = type;
    this->attr.disabled = 0;
  }

  ~PerfCounter() {
    if (fd) {
      close(fd);
    }
  }

  void open(const pid_t pid, const int cpu, const int grp,
            const unsigned long flags);

  void open();

  void start() {
    counting = true;
    asm volatile("nop;"); // pseudo-barrier
    rc = read(fd, &val_begin, sizeof(val_begin));
    asm volatile("nop;"); // pseudo-barrier
    if (rc == -1)
      throw std::runtime_error("Error: PerfData::start -> read counter error");
  }

  void stop() {
    /* if (not counting) */
    /*   throw std::runtime_error("Error: PerfData::end -> counter isn't
     * started"); */
    counting = false;
  }

  uint64_t getValue() {
    if (not counting)
      throw std::runtime_error(
          "Error: PerfData::getValue -> counter isn't started");
    asm volatile("nop;"); // pseudo-barrier
    rc = read(fd, &val_end, sizeof(val_end));
    asm volatile("nop;"); // pseudo-barrier
    if (rc == -1)
      throw std::runtime_error("Error: PerfData::start -> read counter error");

    return val_end - val_begin;
  }
};

} // namespace perf
