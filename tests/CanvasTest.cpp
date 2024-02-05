#define private public
#include "Canvas.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Creating a canvas", "[Canvas]") {
  RT::Canvas c = RT::Canvas(10, 20);
  REQUIRE(c.width == 10);
  REQUIRE(c.height == 20);
  for (auto &p : c.pixels) {
    REQUIRE(p == RT::color(0, 0, 0));
  }
}
TEST_CASE("Writing pixels to a canvas", "[Canvas]") {
  RT::Canvas c = RT::Canvas(10, 20);
  RT::Color red = RT::color(1, 0, 0);
  c.writePixel(2, 3, red);
  REQUIRE(c.pixelAt(2, 3) == red);
}

TEST_CASE("Constructing the PPM header", "[Canvas]") {
  RT::Canvas c = RT::Canvas(5, 3);
  auto ppm = c.PPMHeader();
  REQUIRE(ppm == "P3\n5 3\n255");
}

TEST_CASE("Constructing the PPM pixel data", "[Canvas]") {
  RT::Canvas c = RT::Canvas(5, 3);
  RT::Color c1 = RT::color(1.5, 0, 0);
  RT::Color c2 = RT::color(0, 0.5, 0);
  RT::Color c3 = RT::color(-0.5, 0, 1);
  c.writePixel(0, 0, c1);
  c.writePixel(2, 1, c2);
  c.writePixel(4, 2, c3);
  auto ppm = c.PPMBody();
  REQUIRE(ppm == "255 0 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 128 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 0 0\n"
                 "0 0 255\n");
}