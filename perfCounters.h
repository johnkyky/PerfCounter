#pragma once

#include "perfCounter.h"
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace perf {
namespace {

class HandlerPerfCounter {
public:
  HandlerPerfCounter() = default;
  HandlerPerfCounter(const HandlerPerfCounter &) = delete;
  HandlerPerfCounter &operator=(const HandlerPerfCounter &) = delete;

private:
  std::vector<PerfCounter> counters;
};

} // namespace

enum class PERF_EVENT {
  NB_CYCLES_HW = PERF_COUNT_HW_CPU_CYCLES,
  NB_INSTRUCTIONS_HW = PERF_COUNT_HW_INSTRUCTIONS,
  CACHE_REFERENCES_HW = PERF_COUNT_HW_CACHE_REFERENCES,
  CACHE_MISSES_HW = PERF_COUNT_HW_CACHE_MISSES,
  BRANCH_INSTRUCTIONS_HW = PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
  BRANCH_MISSES_HW = PERF_COUNT_HW_BRANCH_MISSES,

  NB_CYCLES_SW = PERF_COUNT_HW_CPU_CYCLES,
  CPU_CLOCK_SW = PERF_COUNT_SW_CPU_CLOCK,
  TASK_CLOCK_SW = PERF_COUNT_SW_TASK_CLOCK,
  PAGE_FAULTS_SW = PERF_COUNT_SW_PAGE_FAULTS,
  CONTEXT_SWITCHES_SW = PERF_COUNT_SW_CONTEXT_SWITCHES,
  CPU_MIGRATIONS_SW = PERF_COUNT_SW_CPU_MIGRATIONS,
};

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
