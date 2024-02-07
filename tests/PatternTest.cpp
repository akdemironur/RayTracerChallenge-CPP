#include "Pattern.hpp"
#include "Light.hpp"
#include "Shape.hpp"
#include "catch2/catch_test_macros.hpp"
#include <memory>

TEST_CASE("Creating a stripe pattern", "[Pattern]") {
  RT::StripePattern pattern;
  REQUIRE(pattern.a == RT::color(1, 1, 1));
  REQUIRE(pattern.b == RT::color(0, 0, 0));
}

TEST_CASE("A stripe pattern is constant in y", "[Pattern]") {
  RT::StripePattern pattern;
  REQUIRE(pattern.patternAt(RT::point(0, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0, 1, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0, 2, 0)) == RT::color(1, 1, 1));
}

TEST_CASE("A stripe pattern is constant in z", "[Pattern]") {
  RT::StripePattern pattern;
  REQUIRE(pattern.patternAt(RT::point(0, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0, 0, 1)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0, 0, 2)) == RT::color(1, 1, 1));
}

TEST_CASE("A stripe pattern alternates in x", "[Pattern]") {
  RT::StripePattern pattern;
  REQUIRE(pattern.patternAt(RT::point(0, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0.9, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(1, 0, 0)) == RT::color(0, 0, 0));
  REQUIRE(pattern.patternAt(RT::point(-0.1, 0, 0)) == RT::color(0, 0, 0));
  REQUIRE(pattern.patternAt(RT::point(-1, 0, 0)) == RT::color(0, 0, 0));
  REQUIRE(pattern.patternAt(RT::point(-1.1, 0, 0)) == RT::color(1, 1, 1));
}

TEST_CASE("Lighting with a pattern applied", "[Pattern]") {
  RT::Material m;
  RT::StripePattern pattern;
  m.pattern = std::make_unique<RT::StripePattern>(pattern);
  m.ambient = 1;
  m.diffuse = 0;
  m.specular = 0;
  RT::Point eye = RT::point(0, 0, -1);
  RT::Vector normal = RT::vector(0, 0, -1);
  RT::Light light = RT::Light(RT::point(0, 0, -10), RT::color(1, 1, 1));
  RT::Color c1 =
      RT::lighting(m, light, RT::point(0.9, 0, 0), eye, normal, false);
  RT::Color c2 =
      RT::lighting(m, light, RT::point(1.1, 0, 0), eye, normal, false);
  REQUIRE(c1 == RT::color(1, 1, 1));
  REQUIRE(c2 == RT::color(0, 0, 0));
}

TEST_CASE("Stripes with an object transformation", "[Pattern]") {
  RT::Sphere s;
  s.transformation = RT::scaling(2, 2, 2);
  RT::StripePattern pattern;
  RT::Material m;
  REQUIRE(m.pattern == nullptr);
  m.pattern = std::make_unique<RT::StripePattern>(pattern);
  s.material = m;
  RT::Color c = s.patternAt(RT::point(1.5, 0, 0));
  REQUIRE(c == RT::color(1, 1, 1));
}

TEST_CASE("Stripes with a pattern transformation", "[Pattern]") {
  RT::Sphere s;
  RT::StripePattern pattern;
  pattern.transformation = RT::scaling(2, 2, 2);
  RT::Material m;
  m.pattern = std::make_unique<RT::StripePattern>(pattern);
  s.material = m;
  RT::Color c = s.patternAt(RT::point(1.5, 0, 0));
  REQUIRE(c == RT::color(1, 1, 1));
}

TEST_CASE("Stripes with both an object and a pattern transformation",
          "[Pattern]") {
  RT::Sphere s;
  s.transformation = RT::scaling(2, 2, 2);
  RT::StripePattern pattern;
  pattern.transformation = RT::translation(0.5, 0, 0);
  RT::Material m;
  m.pattern = std::make_unique<RT::StripePattern>(pattern);
  s.material = m;
  RT::Color c = s.patternAt(RT::point(2.5, 0, 0));
  REQUIRE(c == RT::color(1, 1, 1));
}