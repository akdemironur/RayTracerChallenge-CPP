#include "Pattern.hpp"
#include <memory>
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
TEST_CASE("The reflected color for a nonreflective material") {
  RT::World w;
  auto r = RT::Ray(RT::point(0, 0, 0), RT::vector(0, 0, 1));
  auto shape = w.objects[1].get();
  shape->material.ambient = 1;
  auto i = RT::Intersection(1, shape);
  auto comps = RT::Computations(i, r);
  auto color = w.reflectedColor(comps);
  REQUIRE(color == RT::color(0, 0, 0));
}

TEST_CASE("The reflected color for a reflective material") {
  RT::World w;
  auto shape = RT::Plane();
  shape.material.reflective = 0.5;
  shape.transformation = RT::translation(0, -1, 0);
  w.add(std::make_unique<RT::Plane>(shape));
  auto r =
      RT::Ray(RT::point(0, 0, -3), RT::vector(0, -sqrt(2) / 2, sqrt(2) / 2));
  auto i = RT::Intersection(sqrt(2), &shape);
  auto comps = RT::Computations(i, r);
  auto color = w.reflectedColor(comps);
  REQUIRE(color == RT::color(0.19032, 0.2379, 0.14274));
}

TEST_CASE("colorAt() with a reflective material") {
  RT::World w;
  auto shape = RT::Plane();
  shape.material.reflective = 0.5;
  shape.transformation = RT::translation(0, -1, 0);
  w.add(std::make_unique<RT::Plane>(shape));
  auto r =
      RT::Ray(RT::point(0, 0, -3), RT::vector(0, -sqrt(2) / 2, sqrt(2) / 2));
  auto i = RT::Intersection(sqrt(2), &shape);
  auto comps = RT::Computations(i, r);
  auto color = w.shadeHit(comps);
  REQUIRE(color == RT::color(0.87677, 0.92436, 0.82918));
}

TEST_CASE("colorAt() with mutually reflective surfaces") {
  RT::World w;
  auto lower = RT::Plane();
  lower.material.reflective = 1;
  lower.transformation = RT::translation(0, -1, 0);
  w.add(std::make_unique<RT::Plane>(lower));
  auto upper = RT::Plane();
  upper.material.reflective = 1;
  upper.transformation = RT::translation(0, 1, 0);
  w.add(std::make_unique<RT::Plane>(upper));
  auto r = RT::Ray(RT::point(0, 0, 0), RT::vector(0, 1, 0));
  auto c = w.colorAt(r);
  auto c2 = w.colorAt(r);
  REQUIRE(c == c2);
}

TEST_CASE("The refracted color with an opaque surface") {
  RT::World w;
  auto shape = w.objects[0].get();
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto xs =
      RT::intersections(RT::Intersection(4, shape), RT::Intersection(6, shape));
  auto comps = RT::Computations(xs[0], r, xs);
  auto c = w.refractedColor(comps);
  REQUIRE(c == RT::color(0, 0, 0));
}

TEST_CASE("The refracted color at the maximum recursive depth") {
  RT::World w;
  auto shape = w.objects[0].get();
  shape->material.transparency = 1.0;
  shape->material.refractiveIndex = 1.5;
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto xs =
      RT::intersections(RT::Intersection(4, shape), RT::Intersection(6, shape));
  auto comps = RT::Computations(xs[0], r, xs);
  auto c = w.refractedColor(comps, 0);
  REQUIRE(c == RT::color(0, 0, 0));
}

TEST_CASE("The refracted color under total internal reflection") {
  RT::World w;
  auto shape = w.objects[0].get();
  shape->material.transparency = 1.0;
  shape->material.refractiveIndex = 1.5;
  auto r = RT::Ray(RT::point(0, 0, sqrt(2) / 2), RT::vector(0, 1, 0));
  auto xs = RT::intersections(RT::Intersection(-sqrt(2) / 2, shape),
                              RT::Intersection(sqrt(2) / 2, shape));
  auto comps = RT::Computations(xs[1], r, xs);
  auto c = w.refractedColor(comps, 5);
  REQUIRE(c == RT::color(0, 0, 0));
}

TEST_CASE("The refracted color with a refracted ray") {
  RT::World w;
  auto a = w.objects[0].get();
  a->material.ambient = 1;
  a->material.pattern = std::make_unique<RT::TestPattern>(RT::TestPattern());
  auto b = w.objects[1].get();
  b->material.transparency = 1.0;
  b->material.refractiveIndex = 1.5;
  auto r = RT::Ray(RT::point(0, 0, 0.1), RT::vector(0, 1, 0));
  auto xs = RT::intersections(
      RT::Intersection(-0.9899, a), RT::Intersection(-0.4899, b),
      RT::Intersection(0.4899, b), RT::Intersection(0.9899, a));
  auto comps = RT::Computations(xs[2], r, xs);
  auto c = w.refractedColor(comps, 5);
  REQUIRE(c == RT::color(0, 0.99888, 0.04725));
}

TEST_CASE("shadeHit() with a transparent material") {
  RT::World w;
  auto floor = RT::Plane();
  floor.transformation = RT::translation(0, -1, 0);
  floor.material.transparency = 0.5;
  floor.material.refractiveIndex = 1.5;
  w.add(std::make_unique<RT::Plane>(floor));
  auto ball = RT::Sphere();
  ball.material.color = RT::color(1, 0, 0);
  ball.material.ambient = 0.5;
  ball.transformation = RT::translation(0, -3.5, -0.5);
  w.add(std::make_unique<RT::Sphere>(ball));
  auto r =
      RT::Ray(RT::point(0, 0, -3), RT::vector(0, -sqrt(2) / 2, sqrt(2) / 2));
  auto xs = RT::intersections(RT::Intersection(sqrt(2), &floor));
  auto comps = RT::Computations(xs[0], r, xs);
  auto c = w.shadeHit(comps, 5);
  REQUIRE(c == RT::color(0.93642, 0.68642, 0.68642));
}

TEST_CASE("shadeHit() with a reflective, transparent material") {
  RT::World w;
  auto floor = RT::Plane();
  floor.transformation = RT::translation(0, -1, 0);
  floor.material.reflective = 0.5;
  floor.material.transparency = 0.5;
  floor.material.refractiveIndex = 1.5;
  w.add(std::make_unique<RT::Plane>(floor));
  auto ball = RT::Sphere();
  ball.material.color = RT::color(1, 0, 0);
  ball.material.ambient = 0.5;
  ball.transformation = RT::translation(0, -3.5, -0.5);
  w.add(std::make_unique<RT::Sphere>(ball));
  auto xs = RT::intersections(RT::Intersection(sqrt(2), &floor));
  auto r =
      RT::Ray(RT::point(0, 0, -3), RT::vector(0, -sqrt(2) / 2, sqrt(2) / 2));
  auto comps = RT::Computations(xs[0], r, xs);
  auto c = w.shadeHit(comps, 5);
  REQUIRE(c == RT::color(0.93391, 0.69643, 0.69243));
}
