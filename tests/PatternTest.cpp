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
  RT::Sphere s;
  s.material = m;
  RT::Point eye = RT::point(0, 0, -1);
  RT::Vector normal = RT::vector(0, 0, -1);
  RT::Light light = RT::Light(RT::point(0, 0, -10), RT::color(1, 1, 1));
  RT::Color c1 = s.lighting(light, RT::point(0.9, 0, 0), eye, normal, false);
  RT::Color c2 = s.lighting(light, RT::point(1.1, 0, 0), eye, normal, false);
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

TEST_CASE("The default pattern transformation", "[Pattern]") {
  RT::TestPattern pattern;
  REQUIRE(pattern.transformation == RT::identityMatrix<4>());
}

TEST_CASE("Assigning a transformation", "[Pattern]") {
  RT::TestPattern pattern;
  pattern.transformation = RT::translation(1, 2, 3);
  REQUIRE(pattern.transformation == RT::translation(1, 2, 3));
}

TEST_CASE("A pattern with an object transformation", "[Pattern]") {
  RT::Sphere s;
  s.transformation = RT::scaling(2, 2, 2);
  RT::TestPattern pattern;
  s.material.pattern = std::make_unique<RT::TestPattern>(pattern);
  RT::Color c = s.patternAt(RT::point(2, 3, 4));
  REQUIRE(c == RT::color(1, 1.5, 2));
}

TEST_CASE("A pattern with a pattern transformation", "[Pattern]") {
  RT::Sphere s;
  RT::TestPattern pattern;
  pattern.transformation = RT::scaling(2, 2, 2);
  s.material.pattern = std::make_unique<RT::TestPattern>(pattern);
  RT::Color c = s.patternAt(RT::point(2, 3, 4));
  REQUIRE(c == RT::color(1, 1.5, 2));
}

TEST_CASE("A pattern with both an object and a pattern transformation",
          "[Pattern]") {
  RT::Sphere s;
  s.transformation = RT::scaling(2, 2, 2);
  RT::TestPattern pattern;
  pattern.transformation = RT::translation(0.5, 1, 1.5);
  s.material.pattern = std::make_unique<RT::TestPattern>(pattern);
  RT::Color c = s.patternAt(RT::point(2.5, 3, 3.5));
  REQUIRE(c == RT::color(0.75, 0.5, 0.25));
}

TEST_CASE("A gradient linearly interpolates between colors", "[Pattern]") {
  RT::GradientPattern pattern;
  REQUIRE(pattern.patternAt(RT::point(0, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0.25, 0, 0)) ==
          RT::color(0.75, 0.75, 0.75));
  REQUIRE(pattern.patternAt(RT::point(0.5, 0, 0)) == RT::color(0.5, 0.5, 0.5));
  REQUIRE(pattern.patternAt(RT::point(0.75, 0, 0)) ==
          RT::color(0.25, 0.25, 0.25));
}

TEST_CASE("A ring should extend in both x and z", "[Pattern]") {
  RT::RingPattern pattern;
  REQUIRE(pattern.patternAt(RT::point(0, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(1, 0, 0)) == RT::color(0, 0, 0));
  REQUIRE(pattern.patternAt(RT::point(0, 0, 1)) == RT::color(0, 0, 0));
  REQUIRE(pattern.patternAt(RT::point(0.708, 0, 0.708)) == RT::color(0, 0, 0));
}

TEST_CASE("Checkers should repeat in x", "[Pattern]") {
  RT::CheckersPattern pattern;
  REQUIRE(pattern.patternAt(RT::point(0, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0.99, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(1.01, 0, 0)) == RT::color(0, 0, 0));
}

TEST_CASE("Checkers should repeat in y", "[Pattern]") {
  RT::CheckersPattern pattern;
  REQUIRE(pattern.patternAt(RT::point(0, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0, 0.99, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0, 1.01, 0)) == RT::color(0, 0, 0));
}

TEST_CASE("Checkers should repeat in z", "[Pattern]") {
  RT::CheckersPattern pattern;
  REQUIRE(pattern.patternAt(RT::point(0, 0, 0)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0, 0, 0.99)) == RT::color(1, 1, 1));
  REQUIRE(pattern.patternAt(RT::point(0, 0, 1.01)) == RT::color(0, 0, 0));
}
