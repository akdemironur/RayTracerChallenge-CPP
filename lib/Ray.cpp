#include "Ray.hpp"

#include <utility>

namespace RT {

Ray::Ray() : origin(point(0, 0, 0)), direction(vector(0, 0, 0)) {}

Ray::Ray(Point origin, Vector direction)
    : origin(std::move(origin)), direction(std::move(direction)) {}

auto Ray::position(double t) const -> Point { return origin + direction * t; }

auto Ray::transform(const Transformation &m) const -> Ray {
  return {m * origin, m * direction};
}

} // namespace RT