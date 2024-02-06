#include "Ray.hpp"

namespace RT {

Ray::Ray() : origin(point(0, 0, 0)), direction(vector(0, 0, 0)) {}

Ray::Ray(Point origin, Vector direction)
    : origin(origin), direction(direction) {}

Point Ray::position(double t) const { return origin + direction * t; }

int Ray::count() const { return intersections.size(); }

Ray transform(const Ray &r, const Transformation &m) {
  return Ray(m * r.origin, m * r.direction);
}

} // namespace RT