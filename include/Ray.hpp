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
  int count() const;

private:
  std::vector<double> intersections;
};

Ray transform(const Ray &r, const Transformation &m);

} // namespace RT