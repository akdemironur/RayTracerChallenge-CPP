#include "Tuple.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Vector, Point", "[Tuple]") {
  RT::Tuple a = RT::Tuple(4.3, -4.2, 3.1, 1.0);
  REQUIRE(a.x == 4.3);
  REQUIRE(a.y == -4.2);
  REQUIRE(a.z == 3.1);
  REQUIRE(a.w == 1.0);
  REQUIRE(a.isPoint() == true);
  REQUIRE(a.isVector() == false);

  RT::Tuple b = RT::Tuple(4.3, -4.2, 3.1, 0.0);
  REQUIRE(b.x == 4.3);
  REQUIRE(b.y == -4.2);
  REQUIRE(b.z == 3.1);
  REQUIRE(b.w == 0.0);
  REQUIRE(b.isPoint() == false);
  REQUIRE(b.isVector() == true);

  RT::Tuple p = RT::point(4, -4, 3);
  REQUIRE(RT::Tuple(4, -4, 3, 1) == p);

  auto v = RT::vector(4, -4, 3);
  REQUIRE(RT::Tuple(4, -4, 3, 0) == v);
}

TEST_CASE("Tuple addition", "[Tuple]") {
  RT::Tuple a1 = RT::Tuple(3, -2, 5, 1);
  RT::Tuple a2 = RT::Tuple(-2, 3, 1, 0);
  REQUIRE(a1 + a2 == RT::Tuple(1, 1, 6, 1));
}

TEST_CASE("Tuple subtraction", "[Tuple]") {
  RT::Tuple a1 = RT::point(3, 2, 1);
  RT::Tuple a2 = RT::vector(5, 6, 7);
  RT::Tuple a3 = RT::vector(3, 2, 1);
  REQUIRE(a1 - a2 == RT::point(-2, -4, -6));
  REQUIRE(a3 - a2 == RT::vector(-2, -4, -6));
}

TEST_CASE("Tuple negation", "[Tuple]") {
  RT::Tuple a = RT::Tuple(1, -2, 3, -4);
  auto zero = RT::Tuple(0, 0, 0, 0);
  REQUIRE(zero - a == -a);
  REQUIRE(-a == RT::Tuple(-1, 2, -3, 4));
}

TEST_CASE("Tuple multiplication by scalar", "[Tuple]") {
  RT::Tuple a = RT::Tuple(1, -2, 3, -4);
  REQUIRE(a * 3.5 == RT::Tuple(3.5, -7, 10.5, -14));
  REQUIRE(a * 0.5 == RT::Tuple(0.5, -1, 1.5, -2));
}

TEST_CASE("Tuple division by scalar", "[Tuple]") {
  RT::Tuple a = RT::Tuple(1, -2, 3, -4);
  REQUIRE(a / 2 == RT::Tuple(0.5, -1, 1.5, -2));
}

TEST_CASE("Vector magnitude", "[Tuple]") {
  REQUIRE(RT::vector(1, 0, 0).magnitude() == 1);
  REQUIRE(RT::vector(0, 1, 0).magnitude() == 1);
  REQUIRE(RT::vector(0, 0, 1).magnitude() == 1);
  REQUIRE(RT::vector(1, 2, 3).magnitude() == std::sqrt(14));
  REQUIRE(RT::vector(-1, -2, -3).magnitude() == std::sqrt(14));
}

TEST_CASE("Vector normalization", "[Tuple]") {
  REQUIRE(RT::vector(4, 0, 0).normalize() == RT::vector(1, 0, 0));
  REQUIRE(RT::vector(1, 2, 3).normalize() ==
          RT::vector(1 / std::sqrt(14), 2 / std::sqrt(14), 3 / std::sqrt(14)));
  REQUIRE(RT::vector(1, 2, 3).normalize().magnitude() == 1);
}

TEST_CASE("Vector dot product", "[Tuple]") {
  RT::Tuple a = RT::vector(1, 2, 3);
  RT::Tuple b = RT::vector(2, 3, 4);
  REQUIRE(dot(a, b) == 20);
}
TEST_CASE("Vector cross product", "[Tuple]") {
  RT::Tuple a = RT::vector(1, 2, 3);
  RT::Tuple b = RT::vector(2, 3, 4);
  REQUIRE(cross(a, b) == RT::vector(-1, 2, -1));
  REQUIRE(cross(b, a) == RT::vector(1, -2, 1));
}

TEST_CASE("Colors are (r,g,b) tuples", "[Tuple]") {
  RT::Tuple c = RT::color(-0.5, 0.4, 1.7);
  REQUIRE(c.red == -0.5);
  REQUIRE(c.green == 0.4);
  REQUIRE(c.blue == 1.7);
}

TEST_CASE("Adding colors", "[Tuple]") {
  RT::Tuple c1 = RT::color(0.9, 0.6, 0.75);
  RT::Tuple c2 = RT::color(0.7, 0.1, 0.25);
  REQUIRE(c1 + c2 == RT::color(1.6, 0.7, 1.0));
}

TEST_CASE("Subtracting colors", "[Tuple]") {
  RT::Tuple c1 = RT::color(0.9, 0.6, 0.75);
  RT::Tuple c2 = RT::color(0.7, 0.1, 0.25);
  REQUIRE(c1 - c2 == RT::color(0.2, 0.5, 0.5));
}

TEST_CASE("Multiplying a color by a scalar", "[Tuple]") {
  RT::Tuple c = RT::color(0.2, 0.3, 0.4);
  REQUIRE(c * 2 == RT::color(0.4, 0.6, 0.8));
}

TEST_CASE("Multiplying colors", "[Tuple]") {
  RT::Tuple c1 = RT::color(1, 0.2, 0.4);
  RT::Tuple c2 = RT::color(0.9, 1, 0.1);
  REQUIRE(hadamard(c1, c2) == RT::color(0.9, 0.2, 0.04));
}

TEST_CASE("Reflecting a vector approaching at 45 degrees", "[Tuple]") {
  RT::Tuple v = RT::vector(1, -1, 0);
  RT::Tuple n = RT::vector(0, 1, 0);
  REQUIRE(v.reflect(n) == RT::vector(1, 1, 0));
}

TEST_CASE("Reflecting a vector off a slanted surface", "[Tuple]") {
  RT::Tuple v = RT::vector(0, -1, 0);
  RT::Tuple n = RT::vector(std::sqrt(2) / 2, std::sqrt(2) / 2, 0);
  REQUIRE(v.reflect(n) == RT::vector(1, 0, 0));
}