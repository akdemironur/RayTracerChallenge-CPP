#pragma once
#include "Light.hpp"
#include "Shape.hpp"
#include <memory>
#include <optional>
#include <variant>
#include <vector>

namespace RT {

class World {
public:
  World(bool defaultWorld = true);
  Light light;
  void add(std::unique_ptr<Shape> object);
  bool contains(const Shape &object) const;
  int count() const;
  std::vector<Intersection> intersect(const Ray &ray) const;
  Color shadeHit(const Computations &comps, int remaining = 5) const;
  Color colorAt(const Ray &ray, int remaining = 5) const;
  Color reflectedColor(const Computations &comps, int remaining = 5) const;
  Color refractedColor(const Computations &comps, int remaining = 5) const;
  bool isShadowed(const Point &point) const;

private:
  std::vector<std::unique_ptr<Shape>> objects;
};

} // namespace RT