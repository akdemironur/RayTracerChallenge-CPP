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
  static constexpr int MAX_RECURSION_DEPTH = 5;

  explicit World(bool defaultWorld = true);
  Light light;
  void add(std::unique_ptr<Shape> object);
  [[nodiscard]] bool contains(const Shape &object) const;
  [[nodiscard]] size_t count() const;
  [[nodiscard]] std::vector<Intersection> intersect(const Ray &ray) const;
  [[nodiscard]] Color shadeHit(const Computations &comps,
                               int remaining = MAX_RECURSION_DEPTH) const;
  [[nodiscard]] Color colorAt(const Ray &ray,
                              int remaining = MAX_RECURSION_DEPTH) const;
  [[nodiscard]] Color reflectedColor(const Computations &comps,
                                     int remaining = MAX_RECURSION_DEPTH) const;
  [[nodiscard]] Color refractedColor(const Computations &comps,
                                     int remaining = MAX_RECURSION_DEPTH) const;
  [[nodiscard]] bool isShadowed(const Point &point) const;

private:
  std::vector<std::unique_ptr<Shape>> objects;
};

} // namespace RT