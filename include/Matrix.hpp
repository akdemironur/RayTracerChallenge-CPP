#pragma once
#include "Tuple.hpp"
#include <array>
#include <cassert>
namespace RT {

template <size_t m> class Matrix {
private:
  std::array<double, m * m> data;

public:
  Matrix();
  explicit Matrix(const std::array<double, m * m> &values);
  const double &operator()(int i, int j) const;
  double &operator()(int i, int j);
  bool operator==(const Matrix<m> &other) const;
  [[nodiscard]] Matrix<m> transpose() const;
  [[nodiscard]] Matrix<m - 1> submatrix(int row, int col) const;
  [[nodiscard]] double minor(int row, int col) const;
  [[nodiscard]] double cofactor(int row, int col) const;
  [[nodiscard]] double determinant() const;
  [[nodiscard]] bool isInvertible() const;
  [[nodiscard]] Matrix<m> inverse() const;
};

using Transformation = Matrix<4>;

template <size_t m> Matrix<m>::Matrix() : data(std::array<double, m * m>(0)) {}

template <size_t m>
Matrix<m>::Matrix(const std::array<double, m * m> &values) : data(values) {}

template <size_t m> double &Matrix<m>::operator()(int i, int j) {
  assert(i >= 0 && i < m && j >= 0 && j < m && "out of bounds");
  return data.at(i * m + j);
}

template <size_t m> const double &Matrix<m>::operator()(int i, int j) const {
  assert(i >= 0 && i < m && j >= 0 && j < m && "out of bounds");
  return data.at(i * m + j);
}

template <size_t m> bool Matrix<m>::operator==(const Matrix<m> &other) const {
  for (auto i = 0; i < m * m; i++) {
    if (!approxEqual(data[i], other.data[i])) {
      return false;
    }
  }
  return true;
}

template <size_t m>
Matrix<m> operator+(const Matrix<m> &a, const Matrix<m> &b) {
  std::array<double, m * m> v;
  for (int i = 0; i < m * m; i++) {
    v[i] = a.data[i] + b.data[i];
  }
  return Matrix<m>(v);
}

template <size_t m>
Matrix<m> operator-(const Matrix<m> &a, const Matrix<m> &b) {
  std::array<double, m * m> v;
  for (int i = 0; i < m * m; i++) {
    v[i] = a.data[i] - b.data[i];
  }
  return Matrix<m>(v);
}

template <size_t m>
Matrix<m> operator*(const Matrix<m> &a, const Matrix<m> &b) {
  std::array<double, m * m> v{};
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      double sum = 0;
      for (int k = 0; k < m; k++) {
        sum += a(i, k) * b(k, j);
      }
      v.at(i * m + j) = sum;
    }
  }
  return Matrix<m>(v);
}

template <size_t m> Tuple operator*(const Matrix<m> &a, const Tuple &b) {
  std::array<double, 4> v{0, 0, 0, 0};
  for (int i = 0; i < m; i++) {
    double sum = 0;
    for (int j = 0; j < m; j++) {
      sum += a(i, j) * b(j);
    }
    v.at(i) = sum;
  }
  return {v[0], v[1], v[2], v[3]};
}

template <size_t m> Matrix<m> identityMatrix() {
  std::array<double, m * m> v{0};
  for (int i = 0; i < m; i++) {
    v.at(i * m + i) = 1;
  }
  return Matrix<m>(v);
}

template <> inline double Matrix<2>::determinant() const {
  return data[0] * data[3] - data[1] * data[2];
}

template <size_t m> double Matrix<m>::determinant() const {
  double det = 0;
  for (int i = 0; i < m; i++) {
    det += data[i] * cofactor(0, i);
  }
  return det;
}

template <size_t m> bool Matrix<m>::isInvertible() const {
  return determinant() != 0;
}

template <size_t m> Matrix<m> Matrix<m>::inverse() const {
  double det = determinant();
  std::array<double, m * m> v;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      v[i * m + j] = cofactor(j, i) / det;
    }
  }
  return Matrix<m>(v);
}

template <size_t m> Matrix<m - 1> Matrix<m>::submatrix(int row, int col) const {
  std::array<double, (m - 1) * (m - 1)> v;
  int k = 0;
  for (int i = 0; i < m; i++) {
    if (i == row) {
      continue;
    }
    for (int j = 0; j < m; j++) {
      if (j == col) {
        continue;
      }
      v[k++] = data[i * m + j];
    }
  }
  return Matrix<m - 1>(v);
}

template <size_t m> double Matrix<m>::minor(int row, int col) const {
  auto sm = submatrix(row, col);
  return sm.determinant();
}

template <size_t m> double Matrix<m>::cofactor(int row, int col) const {
  return minor(row, col) * (1 - 2 * ((row + col) % 2));
}

template <size_t m> Matrix<m> Matrix<m>::transpose() const {
  std::array<double, m * m> v;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      v[i * m + j] = data[j * m + i];
    }
  }
  return Matrix<m>(v);
}

inline Transformation translation(double x, double y, double z) {
  auto m = identityMatrix<4>();
  m(0, 3) = x;
  m(1, 3) = y;
  m(2, 3) = z;
  return m;
}

inline Transformation scaling(double x, double y, double z) {
  auto m = identityMatrix<4>();
  m(0, 0) = x;
  m(1, 1) = y;
  m(2, 2) = z;
  return m;
}

inline Transformation rotationX(double r) {
  auto m = identityMatrix<4>();
  m(1, 1) = std::cos(r);
  m(1, 2) = -std::sin(r);
  m(2, 1) = std::sin(r);
  m(2, 2) = std::cos(r);
  return m;
}

inline Transformation rotationY(double r) {
  auto m = identityMatrix<4>();
  m(0, 0) = std::cos(r);
  m(0, 2) = std::sin(r);
  m(2, 0) = -std::sin(r);
  m(2, 2) = std::cos(r);
  return m;
}

inline Transformation rotationZ(double r) {
  auto m = identityMatrix<4>();
  m(0, 0) = std::cos(r);
  m(0, 1) = -std::sin(r);
  m(1, 0) = std::sin(r);
  m(1, 1) = std::cos(r);
  return m;
}

inline Transformation shearing(double xy, double xz, double yx, double yz,
                               double zx, double zy) {
  auto m = identityMatrix<4>();
  m(0, 1) = xy;
  m(0, 2) = xz;
  m(1, 0) = yx;
  m(1, 2) = yz;
  m(2, 0) = zx;
  m(2, 1) = zy;
  return m;
}

inline Transformation operator>>=(const Transformation &B,
                                  const Transformation &A) {
  return A * B;
}

inline Tuple operator>>=(const Tuple &b, const Transformation &A) {
  return A * b;
}

inline Transformation viewTransform(const Point &from, const Point &to,
                                    const Vector &up) {
  auto forward = (to - from).norm();
  auto left = cross(forward, up.norm());
  auto trueUp = cross(left, forward);
  auto orientation =
      Transformation({left.x, left.y, left.z, 0, trueUp.x, trueUp.y, trueUp.z,
                      0, -forward.x, -forward.y, -forward.z, 0, 0, 0, 0, 1});
  return orientation * translation(-from.x, -from.y, -from.z);
}

} // namespace RT