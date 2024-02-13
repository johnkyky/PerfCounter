#pragma once

#include "perfCounter_LAPI.hpp"
#include <cstdint>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>

namespace perf {

namespace {

static std::unordered_map<std::string, PerfCounters> counters;

} // namespace

void initalize();
void initalize(const HARDWARE_EVENT_TYPE hw_events);
void initalize(const SOFTWARE_EVENT_TYPE sw_events);
void initalize(const std::vector<HARDWARE_EVENT_TYPE> &hw_events);
void initalize(const std::vector<SOFTWARE_EVENT_TYPE> &sw_events);
void initalize(const std::vector<HARDWARE_EVENT_TYPE> &hw_events,
               const std::vector<SOFTWARE_EVENT_TYPE> &sw_events);
void initalize(const std::initializer_list<HARDWARE_EVENT_TYPE> &hw_events);
void initalize(const std::initializer_list<SOFTWARE_EVENT_TYPE> &sw_events);
void initalize(const std::initializer_list<HARDWARE_EVENT_TYPE> &hw_events,
               const std::initializer_list<SOFTWARE_EVENT_TYPE> &sw_events);

void begin();
void begin(const std::string &str_counter);
void begin(const perf::HARDWARE_EVENT_TYPE hw_event);
void begin(const perf::SOFTWARE_EVENT_TYPE sw_event);
void begin(const std::vector<std::string> &str_counters);

std::unordered_map<std::string, uint64_t> end();
uint64_t end(const std::string &str_counter);
uint64_t end(const perf::HARDWARE_EVENT_TYPE hw_event);
uint64_t end(const perf::SOFTWARE_EVENT_TYPE sw_event);
std::unordered_map<std::string, uint64_t>
end(const std::vector<std::string> &str_counters);

void finalize();

} // namespace perf
