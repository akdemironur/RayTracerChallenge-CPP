#include "Camera.hpp"
#include "Util.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructing a camera", "[Camera]") {
  auto hsize = 160;
  auto vsize = 120;
  auto fieldOfView = M_PI / 2;
  RT::Camera c(hsize, vsize, fieldOfView);
  REQUIRE(c.hsize == 160);
  REQUIRE(c.vsize == 120);
  REQUIRE(c.fieldOfView == M_PI / 2);
  REQUIRE(c.transform == RT::identityMatrix<4>());
}

TEST_CASE("The pixel size for a horizontal canvas", "[Camera]") {
  RT::Camera c(200, 125, M_PI / 2);
  REQUIRE(RT::isEqual(c.pixelSize, 0.01));
}

TEST_CASE("The pixel size for a vertical canvas", "[Camera]") {
  RT::Camera c(125, 200, M_PI / 2);
  REQUIRE(RT::isEqual(c.pixelSize, 0.01));
}

TEST_CASE("Constructing a ray through the center of the canvas", "[Camera]") {
  RT::Camera c(201, 101, M_PI / 2);
  auto r = c.rayForPixel(100, 50);
  REQUIRE(r.origin == RT::point(0, 0, 0));
  REQUIRE(r.direction == RT::vector(0, 0, -1));
}

TEST_CASE("Constructing a ray through a corner of the canvas", "[Camera]") {
  RT::Camera c(201, 101, M_PI / 2);
  auto r = c.rayForPixel(0, 0);
  REQUIRE(r.origin == RT::point(0, 0, 0));
  REQUIRE(r.direction == RT::vector(0.66519, 0.33259, -0.66851));
}

TEST_CASE("Constructing a ray when the camera is transformed", "[Camera]") {
  RT::Camera c(201, 101, M_PI / 2);
  c.transform = RT::rotationY(M_PI / 4) * RT::translation(0, -2, 5);
  auto r = c.rayForPixel(100, 50);
  REQUIRE(r.origin == RT::point(0, 2, -5));
  REQUIRE(r.direction == RT::vector(std::sqrt(2) / 2, 0, -std::sqrt(2) / 2));
}

TEST_CASE("Rendering a world with a camera", "[Camera]") {
  RT::World w;
  RT::Camera c(11, 11, M_PI / 2);
  auto from = RT::point(0, 0, -5);
  auto to = RT::point(0, 0, 0);
  auto up = RT::vector(0, 1, 0);
  c.transform = RT::viewTransform(from, to, up);
  auto image = c.render(w);
  REQUIRE(image.pixelAt(5, 5) == RT::color(0.38066, 0.47583, 0.2855));
}