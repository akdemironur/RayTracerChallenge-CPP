#include "Light.hpp"

namespace RT {

Light::Light() : position(point(0, 0, 0)), intensity(color(1, 1, 1)) {}
Light::Light(Point position, Color intensity)
    : position(position), intensity(intensity) {}

bool Light::operator==(const Light &l) const {
  return position == l.position && intensity == l.intensity;
}

bool Light::operator!=(const Light &l) const { return !(*this == l); }

} // namespace RT