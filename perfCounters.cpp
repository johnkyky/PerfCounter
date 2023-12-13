#include "perfCounters.h"

using namespace perf;

namespace {

void openCounters() {
  for (auto &counter : counters) {
    counter.open();
  }
}

} // namespace

void initalize() {
  if (not counters.empty()) {
    return;
  }

  counters.push_back(PerfCounter(HARDWARE_EVENT_TYPE::NB_CYCLES));
  openCounters();
}

void initalize(const std::vector<HARDWARE_EVENT_TYPE> &hw_events) {
  if (not counters.empty()) {
    return;
  }
  counters.reserve(hw_events.size());
  for (const auto &event : hw_events) {
    counters.push_back(PerfCounter(event));
  }
  openCounters();
}

void initalize(const std::vector<SOFTWARE_EVENT_TYPE> &sw_events) {
  if (not counters.empty()) {
    return;
  }
  counters.reserve(sw_events.size());
  for (const auto &event : sw_events) {
    counters.push_back(PerfCounter(event));
  }
  openCounters();
}

void initalize(const std::vector<HARDWARE_EVENT_TYPE> &hw_events,
               const std::vector<SOFTWARE_EVENT_TYPE> &sw_events) {
  if (not counters.empty()) {
    return;
  }
  counters.reserve(hw_events.size() + sw_events.size());
  for (const auto &event : hw_events) {
    counters.push_back(PerfCounter(event));
  }
  for (const auto &event : sw_events) {
    counters.push_back(PerfCounter(event));
  }
  openCounters();
}

void initalize(const std::initializer_list<HARDWARE_EVENT_TYPE> &hw_events) {
  if (not counters.empty()) {
    return;
  }
  counters.reserve(hw_events.size());
  for (const auto &event : hw_events) {
    counters.push_back(PerfCounter(event));
  }
  openCounters();
}

void initalize(const std::initializer_list<SOFTWARE_EVENT_TYPE> &sw_events) {
  if (not counters.empty()) {
    return;
  }
  for (const auto &event : sw_events) {
    counters.push_back(PerfCounter(event));
  }
  openCounters();
}

void initalize(const std::initializer_list<HARDWARE_EVENT_TYPE> &hw_events,
               const std::initializer_list<SOFTWARE_EVENT_TYPE> &sw_events) {
  if (not counters.empty()) {
    return;
  }
  for (const auto &event : hw_events) {
    counters.push_back(PerfCounter(event));
  }
  for (const auto &event : sw_events) {
    counters.push_back(PerfCounter(event));
  }
  openCounters();
}

void begin() {
  for (auto &counter : counters) {
    counter.start();
  }
}

std::vector<uint64_t> end() {
  std::vector<uint64_t> res(counters.size());
  for (uint64_t i = 0; i < counters.size(); ++i) {
    res[i] = counters[i].getValue();
    counters[i].stop();
  }
  return res;
}

void finalize() { counters.clear(); }
