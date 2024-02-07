#include "Ray.hpp"

namespace RT {

Ray::Ray() : origin(point(0, 0, 0)), direction(vector(0, 0, 0)) {}

Ray::Ray(Point origin, Vector direction)
    : origin(origin), direction(direction) {}

Point Ray::position(double t) const { return origin + direction * t; }

Ray Ray::transform(const Transformation &m) const {
  return Ray(m * origin, m * direction);
}

} // namespace RT