#include "Light.hpp"

#include <utility>

namespace RT {

Light::Light() : position(point(0, 0, 0)), intensity(color(1, 1, 1)) {}
Light::Light(Point position, Color intensity)
    : position(std::move(position)), intensity(std::move(intensity)) {}

auto Light::operator==(const Light &otherLight) const -> bool {
  return position == otherLight.position && intensity == otherLight.intensity;
}

auto Light::operator!=(const Light &otherLight) const -> bool {
  return !(*this == otherLight);
}

} // namespace RT