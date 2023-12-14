#include "perfCounters_HAPI.hpp"
#include <cstdint>
#include <stdexcept>

namespace {

void openCounters() {
  for (auto &counter : perf::counters) {
    counter.second.open();
  }
}

std::string eventToString(perf::HARDWARE_EVENT_TYPE hw_event) {
  switch (hw_event) {
  case perf::HARDWARE_EVENT_TYPE::NB_CYCLES:
    return "NB_CYCLES";
  case perf::HARDWARE_EVENT_TYPE::NB_INSTRUCTIONS:
    return "NB_INSTRUCTIONS";
  case perf::HARDWARE_EVENT_TYPE::CACHE_REFERENCES:
    return "CACHE_REFERENCES";
  case perf::HARDWARE_EVENT_TYPE::CACHE_MISSES:
    return "CACHE_MISSES";
  case perf::HARDWARE_EVENT_TYPE::BRANCH_INSTRUCTIONS:
    return "BRANCH_INSTRUCTIONS";
  case perf::HARDWARE_EVENT_TYPE::BRANCH_MISSES:
    return "BRANCH_MISSES";
  default:
    throw std::runtime_error("Error: eventToString -> event not found");
  }
}

std::string eventToString(perf::SOFTWARE_EVENT_TYPE sw_event) {
  switch (sw_event) {
  case perf::SOFTWARE_EVENT_TYPE::CPU_CLOCK:
    return "CPU_CLOCK";
  case perf::SOFTWARE_EVENT_TYPE::TASK_CLOCK:
    return "TASK_CLOCK";
  case perf::SOFTWARE_EVENT_TYPE::PAGE_FAULTS:
    return "PAGE_FAULTS";
  case perf::SOFTWARE_EVENT_TYPE::CONTEXT_SWITCHES:
    return "CONTEXT_SWITCHES";
  case perf::SOFTWARE_EVENT_TYPE::CPU_MIGRATIONS:
    return "CPU_MIGRATIONS";
  default:
    throw std::runtime_error("Error: eventToString -> event not found");
  }
}

} // namespace

void perf::initalize() {
  if (not counters.empty()) {
    return;
  }

  /* counters.push_back(PerfCounter(HARDWARE_EVENT_TYPE::NB_CYCLES)); */
  openCounters();
}

void perf::initalize(const std::vector<HARDWARE_EVENT_TYPE> &hw_events) {
  if (not counters.empty()) {
    return;
  }
  counters.reserve(hw_events.size());
  for (const auto &event : hw_events) {
    counters.emplace(eventToString(event), event);
  }
  openCounters();
}

void perf::initalize(const std::vector<SOFTWARE_EVENT_TYPE> &sw_events) {
  if (not counters.empty()) {
    return;
  }
  counters.reserve(sw_events.size());
  for (const auto &event : sw_events) {
    counters.emplace(eventToString(event), event);
  }
  openCounters();
}

void perf::initalize(const std::vector<HARDWARE_EVENT_TYPE> &hw_events,
                     const std::vector<SOFTWARE_EVENT_TYPE> &sw_events) {
  if (not counters.empty()) {
    return;
  }
  counters.reserve(hw_events.size() + sw_events.size());
  for (const auto &event : hw_events) {
    counters.emplace(eventToString(event), event);
  }
  for (const auto &event : sw_events) {
    counters.emplace(eventToString(event), event);
  }
  openCounters();
}

void perf::initalize(
    const std::initializer_list<HARDWARE_EVENT_TYPE> &hw_events) {
  if (not counters.empty()) {
    return;
  }
  counters.reserve(hw_events.size());
  for (const auto &event : hw_events) {
    counters.emplace(eventToString(event), event);
  }
  openCounters();
}

void perf::initalize(
    const std::initializer_list<SOFTWARE_EVENT_TYPE> &sw_events) {
  if (not counters.empty()) {
    return;
  }
  for (const auto &event : sw_events) {
    counters.emplace(eventToString(event), event);
  }
  openCounters();
}

void perf::initalize(
    const std::initializer_list<HARDWARE_EVENT_TYPE> &hw_events,
    const std::initializer_list<SOFTWARE_EVENT_TYPE> &sw_events) {
  if (not counters.empty()) {
    return;
  }
  for (const auto &event : hw_events) {
    counters.emplace(eventToString(event), event);
  }
  for (const auto &event : sw_events) {
    counters.emplace(eventToString(event), event);
  }
  openCounters();
}

void perf::begin() {
  for (auto &counter : counters) {
    counter.second.start();
  }
}

void perf::begin(const std::string &str_counter) {
  counters.at(str_counter).start();
}

void perf::begin(const perf::HARDWARE_EVENT_TYPE hw_event) {
  counters.at(eventToString(hw_event)).start();
}

void perf::begin(const perf::SOFTWARE_EVENT_TYPE sw_event) {
  counters.at(eventToString(sw_event)).start();
}

void perf::begin(const std::vector<std::string> &str_counters) {
  for (const auto str_counter : str_counters) {
    counters.at(str_counter).start();
  }
}

std::unordered_map<std::string, uint64_t> perf::end() {
  std::unordered_map<std::string, uint64_t> res(counters.size());
  for (auto &counter : counters) {
    res.emplace(counter.first, counter.second.getValue());
  }
  return res;
}

uint64_t perf::end(const std::string &str_counter) {
  return counters.at(str_counter).getValue();
}

uint64_t perf::end(const perf::HARDWARE_EVENT_TYPE hw_event) {
  return counters.at(eventToString(hw_event)).getValue();
}

uint64_t perf::end(const perf::SOFTWARE_EVENT_TYPE sw_event) {
  return counters.at(eventToString(sw_event)).getValue();
}

std::unordered_map<std::string, uint64_t>
perf::end(const std::vector<std::string> &str_counters) {
  std::unordered_map<std::string, uint64_t> res(str_counters.size());
  for (const auto str_counter : str_counters) {
    res.emplace(str_counter, counters.at(str_counter).getValue());
  }
  return res;
}

void perf::finalize() { counters.clear(); }
