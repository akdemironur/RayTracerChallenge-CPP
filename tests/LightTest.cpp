#include "Light.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("A point light has a position and intensity", "[Light]") {
  RT::Color intensity = RT::color(1, 1, 1);
  RT::Point position = RT::point(0, 0, 0);
  RT::Light light(position, intensity);
  REQUIRE(light.position == position);
  REQUIRE(light.intensity == intensity);
}
