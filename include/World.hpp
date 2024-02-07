#pragma once
#include "Intersection.hpp"
#include "Light.hpp"
#include "Shape.hpp"
#include <optional>
#include <vector>

namespace RT {

class World {
public:
  World(bool defaultWorld = true);
  Light light;
  void add(const Sphere &object);
  bool contains(const Sphere &object) const;
  int count() const;
  std::vector<Intersection> intersect(const Ray &ray) const;
  Color shadeHit(const Computations &comps, int remaining = 5) const;
  Color colorAt(const Ray &ray) const;
  bool isShadowed(const Point &point) const;

private:
  std::vector<Sphere> objects;
};

} // namespace RT