#pragma once
#include "Ray.hpp"
#include "Shape.hpp"
#include <optional>
#include <utility>
#include <vector>

namespace RT {
using Intersection = std::pair<double, const Sphere *>;

class Computations {
public:
  Computations(const Intersection &i, const Ray &r);
  double t;
  const Sphere *object;
  Point point;
  Point overPoint;
  Vector eye;
  Vector normal;
  bool inside;
};

std::vector<Intersection> intersect(const Sphere &s, const Ray &r);

template <typename... Intersections>
std::vector<Intersection> intersections(Intersections... is) {
  return std::vector<Intersection>{is...};
}

std::optional<Intersection> hit(const std::vector<Intersection> &xs);

} // namespace RT