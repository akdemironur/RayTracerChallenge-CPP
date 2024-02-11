#pragma once
#include "Tuple.hpp"
namespace RT {

class Light {
public:
  Point position;
  Color intensity;

  Light();
  Light(Point position, Color intensity);
  bool operator==(const Light &otherLight) const;
  bool operator!=(const Light &otherLight) const;
};
} // namespace RT
