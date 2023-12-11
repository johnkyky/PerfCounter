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

class PerfData {
public:
  enum EVENT_TYPE {
    NB_CYCLES = PERF_COUNT_HW_CPU_CYCLES,
    NB_INSTRUCTIONS = PERF_COUNT_HW_INSTRUCTIONS,
    CACHE_REFERENCES = PERF_COUNT_HW_CACHE_REFERENCES,
    CACHE_MISSES = PERF_COUNT_HW_CACHE_MISSES,
    BRANCH_INSTRUCTIONS = PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
    BRANCH_MISSES = PERF_COUNT_HW_BRANCH_MISSES
  };

private:
  struct perf_event_attr attr;
  uint64_t val_begin = 0;
  uint64_t val_end = 0;
  int fd;
  int rc;
  bool counting;

public:
  PerfData() = delete;

  PerfData(const struct perf_event_attr &attr) : counting(false) {
    std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
    this->attr.type = attr.type;
    this->attr.config = attr.config;
    this->attr.disabled = attr.disabled;
  }

  PerfData(const struct perf_event_attr &&attr) : counting(false) {
    this->attr = std::move(attr);
  }

  PerfData(const unsigned int type, const unsigned long long config,
           const unsigned long long disabled)
      : counting(false) {
    std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
    this->attr.type = type;
    this->attr.config = config;
    this->attr.disabled = disabled;
  }

  PerfData(const PerfData::EVENT_TYPE type) : counting(false) {
    std::memset(&this->attr, 0, sizeof(struct perf_event_attr));
    /* this->attr.type = PERF_TYPE_SOFTWARE; */
    this->attr.type = PERF_TYPE_HARDWARE;
    this->attr.config = type;
    this->attr.disabled = 0;
  }

  ~PerfData() {
    if (fd) {
      close(fd);
    }
  }

  void open(const pid_t pid, const int cpu, const int grp,
            const unsigned long flags) {
    asm volatile("nop;"); // pseudo-barrier
    fd = syscall(__NR_perf_event_open, &attr, pid, cpu, grp, flags);
    asm volatile("nop;"); // pseudo-barrier
    if (fd < 0)
      throw std::runtime_error("Error: Open PerfData::open");
  }

  void open() { this->open(getpid(), -1, -1, 0); }

  void start() {
    counting = true;
    asm volatile("nop;"); // pseudo-barrier
    rc = read(fd, &val_begin, sizeof(val_begin));
    asm volatile("nop;"); // pseudo-barrier
    if (rc == -1)
      throw std::runtime_error("Error: PerfData::start -> read counter error");
  }

  void end() {
    if (not counting)
      throw std::runtime_error("Error: PerfData::end -> counter isn't started");
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
