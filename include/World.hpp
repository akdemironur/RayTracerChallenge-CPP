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
  [[nodiscard]] auto contains(const Shape &object) const -> bool;
  [[nodiscard]] auto count() const -> size_t;
  [[nodiscard]] auto intersect(const Ray &ray) const
      -> std::vector<Intersection>;
  [[nodiscard]] auto shadeHit(const Computations &comps,
                              int remaining = MAX_RECURSION_DEPTH) const
      -> Color;
  [[nodiscard]] auto colorAt(const Ray &ray,
                             int remaining = MAX_RECURSION_DEPTH) const
      -> Color;
  [[nodiscard]] auto reflectedColor(const Computations &comps,
                                    int remaining = MAX_RECURSION_DEPTH) const
      -> Color;
  [[nodiscard]] auto refractedColor(const Computations &comps,
                                    int remaining = MAX_RECURSION_DEPTH) const
      -> Color;
  [[nodiscard]] auto isShadowed(const Point &point) const -> bool;

private:
  std::vector<std::unique_ptr<Shape>> objects;
};

} // namespace RT