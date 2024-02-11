#pragma once
#include "Tuple.hpp"
namespace RT {

class Light {
public:
  Point position;
  Color intensity;

  Light();
  Light(Point position, Color intensity);
  auto operator==(const Light &otherLight) const -> bool;
  auto operator!=(const Light &otherLight) const -> bool;
};
} // namespace RT
