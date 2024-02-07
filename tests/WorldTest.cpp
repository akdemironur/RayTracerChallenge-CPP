#define private public
#include "World.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Creating a world") {
  RT::World w;
  REQUIRE(w.light.position == RT::point(-10, 10, -10));
  REQUIRE(w.light.intensity == RT::color(1, 1, 1));
  REQUIRE(w.count() == 2);

  auto s1 = RT::Sphere();
  s1.material.color = RT::color(0.8, 1.0, 0.6);
  s1.material.diffuse = 0.7;
  s1.material.specular = 0.2;

  auto s2 = RT::Sphere();
  s2.transformation = RT::scaling(0.5, 0.5, 0.5);

  REQUIRE(w.contains(s1));
  REQUIRE(w.contains(s2));
}

TEST_CASE("Intersect a world with a ray") {
  RT::World w;
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto xs = w.intersect(r);
  REQUIRE(xs.size() == 4);
  REQUIRE(xs[0].first == 4);
  REQUIRE(xs[1].first == 4.5);
  REQUIRE(xs[2].first == 5.5);
  REQUIRE(xs[3].first == 6);
}

TEST_CASE("Shading an intersection") {
  RT::World w;
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto shape = w.objects[0].get();
  auto i = RT::Intersection(4, shape);
  auto comps = RT::Computations(i, r);
  auto c = w.shadeHit(comps);
  REQUIRE(c == RT::color(0.38066, 0.47583, 0.2855));
}

TEST_CASE("Shading an intersection from the inside") {
  RT::World w;
  w.light = RT::Light(RT::point(0, 0.25, 0), RT::color(1, 1, 1));
  auto r = RT::Ray(RT::point(0, 0, 0), RT::vector(0, 0, 1));
  auto shape = w.objects[1].get();
  auto i = RT::Intersection(0.5, shape);
  auto comps = RT::Computations(i, r);
  auto c = w.shadeHit(comps);
  REQUIRE(c == RT::color(0.90498, 0.90498, 0.90498));
}

TEST_CASE("The color when a ray misses") {
  RT::World w;
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 1, 0));
  auto c = w.colorAt(r);
  REQUIRE(c == RT::color(0, 0, 0));
}

TEST_CASE("The color when a ray hits") {
  RT::World w;
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto c = w.colorAt(r);
  REQUIRE(c == RT::color(0.38066, 0.47583, 0.2855));
}

TEST_CASE("The color with an intersection behind the ray") {
  RT::World w;
  auto outer = w.objects[0].get();
  outer->material.ambient = 1;
  auto inner = w.objects[1].get();
  inner->material.ambient = 1;
  auto r = RT::Ray(RT::point(0, 0, 0.75), RT::vector(0, 0, -1));
  auto c = w.colorAt(r);
  REQUIRE(c == inner->material.color);
}

TEST_CASE("There is no shadow when nothing is collinear with point and light") {
  RT::World w;
  auto p = RT::point(0, 10, 0);
  REQUIRE(!w.isShadowed(p));
}

TEST_CASE("The shadow when an object is between the point and the light") {
  RT::World w;
  auto p = RT::point(10, -10, 10);
  REQUIRE(w.isShadowed(p));
}

TEST_CASE("There is no shadow when an object is behind the light") {
  RT::World w;
  auto p = RT::point(-20, 20, -20);
  REQUIRE(!w.isShadowed(p));
}

TEST_CASE("There is no shadow when an object is behind the point") {
  RT::World w;
  auto p = RT::point(-2, 2, -2);
  REQUIRE(!w.isShadowed(p));
}

TEST_CASE("shadeHit() is given an intersection in shadow") {
  RT::World w(false);
  w.light = RT::Light(RT::point(0, 0, -10), RT::color(1, 1, 1));
  auto s1 = RT::Sphere();
  w.add(std::make_unique<RT::Sphere>(
      s1)); // Fix: Allocate an object of the concrete class RT::Sphere
  auto s2 = RT::Sphere();
  s2.transformation = RT::translation(0, 0, 10);
  w.add(std::make_unique<RT::Sphere>(s2));
  auto r = RT::Ray(RT::point(0, 0, 5), RT::vector(0, 0, 1));
  auto i = RT::Intersection(4, &s2);
  auto comps = RT::Computations(i, r);
  auto c = w.shadeHit(comps);
  REQUIRE(c == RT::color(0.1, 0.1, 0.1));
}
