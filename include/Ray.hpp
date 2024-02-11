#pragma once
#include "Matrix.hpp"
#include "Tuple.hpp"
#include <vector>

namespace RT {
class Ray {
public:
  Point origin;
  Vector direction;

  Ray();
  Ray(Tuple origin, Tuple direction);
  [[nodiscard]] auto position(double t) const -> Point;
  [[nodiscard]] auto transform(const Transformation &m) const -> Ray;
};

} // namespace RT