#pragma once
#include "Tuple.hpp"
namespace RT {

class Light {
public:
  Point position;
  Color intensity;

  Light();
  Light(Point position, Color intensity);
  bool operator==(const Light &l) const;
  bool operator!=(const Light &l) const;
};
} // namespace RT
