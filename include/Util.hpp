#pragma once
#include <cmath>
#define EPSILON (1e-4)
namespace RT {
template <typename T> auto approxEqual(const T &a, const T &b) -> bool {
  return std::abs(a - b) < EPSILON;
}
} // namespace RT
