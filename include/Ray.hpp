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
  Point position(double t) const;
  Ray transform(const Transformation &m) const;
};

} // namespace RT