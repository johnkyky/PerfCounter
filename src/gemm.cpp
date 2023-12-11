#include "algebra.h"
#include <filesystem>
#include <iostream>

using namespace algebra;

int main(void) {
  std::cout << "hello world!" << std::endl;

  algebra::Matrix_t<float> A(10, 10), B(10, 10), C(10, 10);

  for (size_t i = 0; i < A.getNbRow(); i++) {
    for (size_t j = 0; j < A.getNbCol(); j++) {
      A(i, j) = static_cast<float>(i);
      B(i, j) = static_cast<float>(j);
      C(i, j) = 0.f;
    }
  }

  A.saveToFile(std::filesystem::current_path() / "A2.mat");
  B.saveToFile(std::filesystem::current_path() / "B2.mat");

  algebra::AlgebraOp::gemm(1.1f, A, B, 0.f, C);

  C.saveToFile(std::filesystem::current_path() / "C2.mat");

  return 0;
}
