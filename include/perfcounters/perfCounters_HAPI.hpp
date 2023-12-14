#pragma once

#include "perfCounter.h"
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace perf {
/* namespace { */

class HandlerPerfCounter {
public:
  HandlerPerfCounter() = default;
  HandlerPerfCounter(const HandlerPerfCounter &) = delete;
  HandlerPerfCounter &operator=(const HandlerPerfCounter &) = delete;

private:
  std::vector<PerfCounter> counters;
};

/* } // namespace */

static std::vector<PerfCounter> counters;

void initalize();
void initalize(const std::vector<HARDWARE_EVENT_TYPE> &hw_events);
void initalize(const std::vector<SOFTWARE_EVENT_TYPE> &sw_events);
void initalize(const std::vector<HARDWARE_EVENT_TYPE> &hw_events,
               const std::vector<SOFTWARE_EVENT_TYPE> &sw_events);
void initalize(const std::initializer_list<HARDWARE_EVENT_TYPE> &hw_events);
void initalize(const std::initializer_list<SOFTWARE_EVENT_TYPE> &sw_events);
void initalize(const std::initializer_list<HARDWARE_EVENT_TYPE> &hw_events,
               const std::initializer_list<SOFTWARE_EVENT_TYPE> &sw_events);

void begin();
std::vector<uint64_t> end();

void finalize();

} // namespace perf
