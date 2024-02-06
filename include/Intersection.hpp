#pragma once
#include "Ray.hpp"
#include "Shape.hpp"
#include <optional>
#include <utility>
#include <vector>

namespace RT {
using Intersection = std::pair<double, const Sphere &>;

std::vector<Intersection> intersect(const Sphere &s, const Ray &r);

template <typename... Intersections>
std::vector<Intersection> intersections(Intersections... is) {
  return std::vector<Intersection>{is...};
}

std::optional<Intersection> hit(const std::vector<Intersection> &xs);

} // namespace RT