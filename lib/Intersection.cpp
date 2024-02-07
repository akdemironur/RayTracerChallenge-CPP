#include "Intersection.hpp"

namespace RT {

std::vector<Intersection> intersect(const Sphere &s, const Ray &r) {
  std::vector<Intersection> xs;
  auto ray = r.transform(s.transformation.inverse());
  auto sphere_to_ray = ray.origin - point(0, 0, 0);
  auto a = dot(ray.direction, ray.direction);
  auto b = 2 * dot(ray.direction, sphere_to_ray);
  auto c = dot(sphere_to_ray, sphere_to_ray) - 1;
  auto discriminant = b * b - 4 * a * c;
  if (discriminant >= 0) {
    xs.push_back(Intersection((-b - sqrt(discriminant)) / (2 * a), &s));
    xs.push_back(Intersection((-b + sqrt(discriminant)) / (2 * a), &s));
  }
  return xs;
}

std::optional<Intersection> hit(const std::vector<Intersection> &xs) {
  std::optional<Intersection> result;
  for (const auto &i : xs) {
    if (i.first >= 0) {
      if (result) {
        if (i.first < result->first) {
          result.emplace(i.first, i.second);
        }
      } else {
        result.emplace(i.first, i.second);
      }
    }
  }
  return result;
}

Computations::Computations(const Intersection &i, const Ray &r) {
  t = i.first;
  object = i.second;
  point = r.position(t);
  eye = -r.direction;
  normal = object->normalAt(point);
  if (dot(eye, normal) < 0) {
    inside = true;
    normal = -normal;
  } else {
    inside = false;
  }
  overPoint = point + normal * EPSILON;
}
} // namespace RT
