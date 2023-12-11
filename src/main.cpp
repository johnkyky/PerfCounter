#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "algebra.h"
#include "perf.h"

using namespace perf;

void test(const size_t N) {

  //
  auto vec = algebra::Vector_t<double>(N);
  auto mat = algebra::Matrix_t<double>(N, N);

  PerfData counter_cycle(PerfData::EVENT_TYPE::NB_CYCLES);
  PerfData counter_instruction(PerfData::EVENT_TYPE::NB_INSTRUCTIONS);

  //
  counter_cycle.open();
  counter_instruction.open();

  counter_cycle.start();
  counter_instruction.start();

  vec.randomFill(0.0, 1.0);
  mat.randomFill(0.0, 1.0);

  uint64_t cycles = counter_cycle.getValue();
  uint64_t instructions = counter_instruction.getValue();
  std::cout << "Initialization matrix, vector take : " << cycles
            << " cycles for " << instructions << " instructions (CPI: "
            << static_cast<float>(cycles) / static_cast<float>(instructions)
            << ")" << std::endl;

  counter_cycle.start();
  counter_instruction.start();

  auto out_vec = algebra::Vector_t<double>(N);

  algebra::AlgebraOp::gemv<double>(1.0, mat, vec, 0.0, out_vec);

  cycles = counter_cycle.getValue();
  instructions = counter_instruction.getValue();
  std::cout << "GEMV take : " << cycles << " cycles for " << instructions
            << " instructions (CPI: "
            << static_cast<float>(cycles) / static_cast<float>(instructions)
            << ")" << std::endl;

  counter_cycle.start();
  counter_instruction.start();

  out_vec.saveToFile("out_vec");

  cycles = counter_cycle.getValue();
  instructions = counter_instruction.getValue();
  std::cout << "Save vector restat take : " << cycles << " cycles for "
            << instructions << " instructions (CPI: "
            << static_cast<float>(cycles) / static_cast<float>(instructions)
            << ")" << std::endl;
}

template <typename T>
void saveBench(const std::string &file_name,
               const std::filesystem::path &directory,
               const std::vector<T> &indexes, const std::vector<T> &data) {
  if (!std::filesystem::exists(directory))
    std::filesystem::create_directory(directory);
  std::filesystem::path path = directory / file_name;

  std::ofstream file(path);

  for (size_t i = 0; i < data.size(); i++) {
    file << indexes[i] << " " << data[i] << "\n";
  }
}

void bench_gemm(const size_t size_begin, const size_t size_end,
                const size_t stride, const size_t nb_iteration,
                const std::filesystem::path &output_directory) {
  //
  PerfData counter_cycle(PerfData::EVENT_TYPE::NB_CYCLES);
  PerfData counter_instruction(PerfData::EVENT_TYPE::NB_INSTRUCTIONS);

  counter_cycle.open();
  counter_instruction.open();

  //
  std::vector<uint64_t> cycles;
  std::vector<uint64_t> instructions;
  std::vector<uint64_t> sizes;

  size_t index = 0;
  for (size_t size = size_begin; size < size_end; size += stride) {
    //
    algebra::Matrix_t<double> A(size, size);
    algebra::Matrix_t<double> B(size, size);
    algebra::Matrix_t<double> C(size, size);

    A.randomFill(0.0, 1.0);
    B.randomFill(0.0, 1.0);
    C.randomFill(0.0, 1.0);

    uint64_t tmp_cycles = 0;
    uint64_t tmp_instructions = 0;

    for (size_t i = 0; i < nb_iteration; i++) {
      counter_cycle.start();
      counter_instruction.start();

      asm volatile("nop;"); // pseudo-barrier
      algebra::AlgebraOp::gemm_no_opt<double>(1.0, A, B, 0.001, C);
      asm volatile("nop;"); // pseudo-barrier

      tmp_cycles += counter_cycle.getValue();
      tmp_instructions += counter_instruction.getValue();
      std::cout << "\t\t" << tmp_cycles << " " << tmp_instructions << std::endl;
    }

    std::cout << "size: " << size << " cycles: " << tmp_cycles / nb_iteration
              << " instructions: " << tmp_instructions / nb_iteration
              << std::endl;
    cycles.push_back(tmp_cycles / nb_iteration);
    instructions.push_back(tmp_instructions / nb_iteration);
    sizes.push_back(size);

    index++;
  }

  saveBench("cycles.data", output_directory, sizes, cycles);
  saveBench("instructions.data", output_directory, sizes, instructions);
}

void bench(const size_t size_begin, const size_t size_end) {
  /* PerfData counter_cycle(PerfData::EVENT_TYPE::NB_CYCLES); */
  struct perf_event_attr attr;
  std::memset(&attr, 0, sizeof(struct perf_event_attr));
  attr.type = PERF_TYPE_HARDWARE;
  attr.config = PERF_COUNT_HW_CPU_CYCLES;
  attr.disabled = 0;

  /* counter_cycle.open(); */
  asm volatile("nop;"); // pseudo-barrier
  int fd = syscall(__NR_perf_event_open, &attr, getppid(), -1, -1, 0);
  asm volatile("nop;"); // pseudo-barrier
  if (fd < 0)
    throw std::runtime_error("Error: Open PerfData::open");

  for (size_t size = size_begin; size < size_end; size++) {
    //
    algebra::Matrix_t<double> A(size, size);
    algebra::Matrix_t<double> B(size, size);
    algebra::Matrix_t<double> C(size, size);

    A.randomFill(0.0, 1.0);
    B.randomFill(0.0, 1.0);
    C.randomFill(0.0, 1.0);

    uint64_t val_begin = 0;
    uint64_t val_end = 0;

    /* counter_cycle.start(); */
    asm volatile("nop;"); // pseudo-barrier
    int rc = read(fd, &val_begin, sizeof(val_begin));
    asm volatile("nop;"); // pseudo-barrier
    if (rc == -1)
      throw std::runtime_error("Error: PerfData::start -> read counter error");

    algebra::AlgebraOp::gemm_no_opt<double>(1.0, A, B, 0.001, C);

    // tmp_cycles += counter_cycle.getValue();
    asm volatile("nop;"); // pseudo-barrier
    rc = read(fd, &val_end, sizeof(val_end));
    asm volatile("nop;"); // pseudo-barrier
    if (rc == -1)
      throw std::runtime_error("Error: PerfData::start -> read counter error");

    std::cout << "size: " << size << " cycles: " << val_end - val_begin
              << std::endl;
  }
}

int main(int argc, char **argv) {
  size_t N = 100;
  if (argc > 1)
    N = std::atoll(argv[1]);

  bench_gemm(10, 500, 10, 5, std::filesystem::current_path() / "data");
  // test(N);

  // bench(5, 15);

  return 0;
}
