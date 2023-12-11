#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

namespace algebra {

template <typename T> class AlgebraBase {
public:
  AlgebraBase(const size_t n, const bool fill_zero = false) : data_size(n) {
    data = (T *)malloc(sizeof(T) * n);
    if (not data) {
      throw std::runtime_error(
          "Error: AlgebraBase::AlgebraBase -> Can't alloc data");
    }

    if (fill_zero) {
      for (size_t i = 0; i < n; i++)
        data[i] = 0;
    }
  }

  virtual ~AlgebraBase() { delete data; }

  virtual void print(std::ostream &) const = 0;

  //    template<typename T>
  friend std::ostream &operator<<(std::ostream &os, const AlgebraBase<T> &obj) {
    obj.print(os);
    return os;
  }

  void randomFill(const T min, const T max) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<T> distribution(min, max);

    for (size_t i = 0; i < data_size; i++)
      this->data[i] = distribution(gen);
  }

  virtual void saveToFile(const std::filesystem::path &path) const = 0;

protected:
  size_t data_size;
  T *data;
};

template <typename T> class Vector_t : public AlgebraBase<T> {
public:
  Vector_t(const size_t n, const bool fill_zero = false)
      : AlgebraBase<T>(n, fill_zero), n(n) {}
  ~Vector_t() = default;

  inline const size_t getSize() const { return n; }

  inline T &operator()(const size_t i) {
    const size_t index = i;
    // assert(index >= n);
    return this->data[index];
  }

  inline const T &operator()(const size_t i) const {
    const size_t index = i;
    // assert(index >= n);
    return this->data[index];
  }

  void print(std::ostream &os) const {
    for (size_t i = 0; i < n; i++)
      os << this->operator()(i) << "\n";
  }

  void saveToFile(const std::filesystem::path &path) const {
    std::ofstream file(path);

    file << n << "\n";

    for (size_t i = 0; i < n; i++) {
      file << this->operator()(i) << "\n";
    }
  }

private:
  const size_t n;
};

template <typename T> class Matrix_t : public AlgebraBase<T> {
public:
  Matrix_t(const size_t n, const size_t m, const bool fill_zero = false)
      : AlgebraBase<T>(n * m, fill_zero), n(n), m(m) {}
  /* Matrix_t(const size_t n, const bool fill_zero = false) */
  /*     : AlgebraBase<T>(n * n, fill_zero), n(n), m(n) {} */
  ~Matrix_t() = default;

  inline const size_t getNbRow() const { return n; }
  inline const size_t getNbCol() const { return m; }

  inline T &operator()(const size_t i, const size_t j) {
    const size_t index = i * m + j;
    // assert(index >= n * m);
    return this->data[index];
  }

  inline const T &operator()(const size_t i, const size_t j) const {
    const size_t index = i * m + j;
    // assert(index >= n * m);
    return this->data[index];
  }

  void print(std::ostream &os) const {
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < m; j++) {
        os << this->operator()(i, j) << " ";
      }
      os << "\n";
    }
  }

  void saveToFile(const std::filesystem::path &path) const {
    std::ofstream file(path);

    file << n << " " << m << "\n";

    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < m; j++) {
        file << this->operator()(i, j) << " ";
      }
      file << "\n";
    }
  }

private:
  const size_t n, m;
};

class AlgebraOp {
public:
  AlgebraOp() = delete;

  // alpha * A * B + beta * C = C
  template <typename T>
  static void gemm_no_opt(const T alpha, const Matrix_t<T> &A,
                          const Matrix_t<T> &B, const T beta, Matrix_t<T> &C) {

    for (size_t i = 0; i < A.getNbRow(); i++) {
      for (size_t j = 0; j < B.getNbCol(); j++) {
        T sum = T();
        for (size_t k = 0; k < A.getNbCol(); k++) {
          sum += A(i, k) * B(k, j);
        }
        C(i, j) = alpha * sum + beta * C(i, j);
      }
    }
  }

  // alpha * A * B + beta * C = C
  template <typename T>
  static void gemm(const T alpha, const Matrix_t<T> &A, const Matrix_t<T> &B,
                   const T beta, Matrix_t<T> &C) {

    for (size_t i = 0; i < C.getNbRow(); i++) {
      for (size_t j = 0; j < C.getNbRow(); j++) {
        C(i, j) *= beta;
      }
    }

    for (size_t i = 0; i < A.getNbRow(); i++) {
      for (size_t k = 0; k < A.getNbCol(); k++) {
        for (size_t j = 0; j < B.getNbCol(); j++) {
          C(i, j) += alpha * A(i, k) * B(k, j);
        }
      }
    }
  }

  // alpha * A * x + beta * y = y
  template <typename T>
  static void gemv(const T alpha, const Matrix_t<T> &A, const Vector_t<T> &x,
                   const T beta, Vector_t<T> &y) {
    // assert(A.n == x.n);
    // assert(x.n == y.n);
    for (size_t row = 0; row < A.getNbRow(); row++) {
      T acc = T();
      for (size_t col = 0; col < A.getNbCol(); col++) {
        acc += A(row, col) * x(col);
      }
      y(row) = alpha * acc + beta * y(row);
    }
  }
};

} // namespace algebra
