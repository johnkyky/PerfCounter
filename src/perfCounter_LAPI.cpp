#include "perfCounter_LAPI.hpp"

#include <cstring>
#include <stdexcept>
using namespace perf;

PerfCounter::PerfCounter(const struct perf_event_attr &attr) : counting(false) {
  std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
  this->attr.type = attr.type;
  this->attr.config = attr.config;
  this->attr.disabled = attr.disabled;
}

PerfCounter::PerfCounter(const struct perf_event_attr &&attr)
    : counting(false) {
  this->attr = std::move(attr);
}

PerfCounter::PerfCounter(const unsigned int type,
                         const unsigned long long config,
                         const unsigned long long disabled)
    : counting(false) {
  std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
  this->attr.type = type;
  this->attr.config = config;
  this->attr.disabled = disabled;
}

PerfCounter::PerfCounter(const HARDWARE_EVENT_TYPE type) : counting(false) {
  std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
  this->attr.type = PERF_TYPE_HARDWARE;
  this->attr.config = type;
  this->attr.disabled = 0;
}

PerfCounter::PerfCounter(const SOFTWARE_EVENT_TYPE type) : counting(false) {
  std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
  this->attr.type = PERF_TYPE_SOFTWARE;
  this->attr.config = type;
  this->attr.disabled = 0;
}

PerfCounter::~PerfCounter() {
  if (fd) {
    close(fd);
  }
}

void PerfCounter::open(const pid_t pid, const int cpu, const int grp,
                       const unsigned long flags) {
  asm volatile("nop;"); // pseudo-barrier
  fd = syscall(__NR_perf_event_open, &attr, pid, cpu, grp, flags);
  asm volatile("nop;"); // pseudo-barrier
  if (fd < 0)
    throw std::runtime_error("Error: PerfData::open -> open counter error");
}

void PerfCounter::open() { this->open(getpid(), -1, -1, 0); }

void PerfCounter::start() {
  counting = true;
  asm volatile("nop;"); // pseudo-barrier
  rc = read(fd, &val_begin, sizeof(val_begin));
  asm volatile("nop;"); // pseudo-barrier
  if (rc == -1)
    throw std::runtime_error("Error: PerfData::start -> read counter error");
}

void PerfCounter::stop() {
  /* if (not counting) */
  /*   throw std::runtime_error("Error: PerfData::end -> counter isn't
   * started"); */
  counting = false;
}

uint64_t PerfCounter::getValue() {
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
