#pragma once

#include "perfCounter_LAPI.hpp"
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace perf {

namespace {

std::vector<PerfCounter> counters;

}

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
