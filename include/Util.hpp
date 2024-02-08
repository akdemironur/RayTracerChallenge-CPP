#pragma once
#include <cmath>
#define EPSILON (1e-4)
namespace RT {
template <typename T> bool isEqual(const T &a, const T &b) {
  return std::abs(a - b) < EPSILON;
}
} // namespace RT
