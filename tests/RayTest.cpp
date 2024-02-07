#include "Ray.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Creating and querying a ray", "[Ray]") {
  RT::Point origin = RT::point(1, 2, 3);
  RT::Vector direction = RT::vector(4, 5, 6);
  RT::Ray r(origin, direction);
  REQUIRE(r.origin == origin);
  REQUIRE(r.direction == direction);
}

TEST_CASE("Computing a point from a distance", "[Ray]") {
  RT::Ray r(RT::point(2, 3, 4), RT::vector(1, 0, 0));
  REQUIRE(r.position(0) == RT::point(2, 3, 4));
  REQUIRE(r.position(1) == RT::point(3, 3, 4));
  REQUIRE(r.position(-1) == RT::point(1, 3, 4));
  REQUIRE(r.position(2.5) == RT::point(4.5, 3, 4));
}

TEST_CASE("A ray intersects a sphere at two points", "[Ray]") {
  RT::Ray r(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  RT::Sphere s;
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 4.0);
  REQUIRE(xs[1].first == 6.0);
}

TEST_CASE("A ray intersects a sphere at a tangent", "[Ray]") {
  RT::Ray r(RT::point(0, 1, -5), RT::vector(0, 0, 1));
  RT::Sphere s;
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 5.0);
  REQUIRE(xs[1].first == 5.0);
}

TEST_CASE("A ray misses a sphere", "[Ray]") {
  RT::Ray r(RT::point(0, 2, -5), RT::vector(0, 0, 1));
  RT::Sphere s;
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 0);
}

TEST_CASE("A ray originates inside a sphere", "[Ray]") {
  RT::Ray r(RT::point(0, 0, 0), RT::vector(0, 0, 1));
  RT::Sphere s;
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == -1.0);
  REQUIRE(xs[1].first == 1.0);
}

TEST_CASE("A sphere is behind a ray", "[Ray]") {
  RT::Ray r(RT::point(0, 0, 5), RT::vector(0, 0, 1));
  RT::Sphere s;
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == -6.0);
  REQUIRE(xs[1].first == -4.0);
}

TEST_CASE("An intersection encapsulates t and object", "[Intersection]") {
  RT::Sphere s;
  RT::Intersection i(3.5, &s);
  REQUIRE(i.first == 3.5);
  REQUIRE(i.second == &s);
}

TEST_CASE("Aggregating intersections", "[Intersection]") {
  RT::Sphere s;
  RT::Intersection i1(1, &s);
  RT::Intersection i2(2, &s);
  auto xs = RT::intersections(i1, i2);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 1);
  REQUIRE(xs[1].first == 2);
}

TEST_CASE("Intersect sets the object on the intersection", "[Intersection]") {
  RT::Ray r(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  RT::Sphere s;
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].second == &s);
  REQUIRE(xs[1].second == &s);
}

TEST_CASE("The hit, when all intersections have positive t", "[Intersection]") {
  RT::Sphere s;
  RT::Intersection i1(1, &s);
  RT::Intersection i2(2, &s);
  auto xs = RT::intersections(i1, i2);
  auto i = RT::hit(xs);
  REQUIRE(i.has_value());
  REQUIRE(i.value().first == 1);
  REQUIRE(i.value().second == &s);
}

TEST_CASE("The hit, when some intersections have negative t",
          "[Intersection]") {
  RT::Sphere s;
  RT::Intersection i1(-1, &s);
  RT::Intersection i2(1, &s);
  auto xs = RT::intersections(i1, i2);
  auto i = RT::hit(xs);
  REQUIRE(i.has_value());
  REQUIRE(i.value().first == 1);
  REQUIRE(i.value().second == &s);
}

TEST_CASE("The hit, when all intersections have negative t", "[Intersection]") {
  RT::Sphere s;
  RT::Intersection i1(-2, &s);
  RT::Intersection i2(-1, &s);
  auto xs = RT::intersections(i1, i2);
  auto i = RT::hit(xs);
  REQUIRE(!i.has_value());
}

TEST_CASE("The hit is always the lowest nonnegative intersection",
          "[Intersection]") {
  RT::Sphere s1;
  RT::Intersection i1(5, &s1);
  RT::Sphere s2;
  RT::Intersection i2(7, &s2);
  RT::Sphere s3;
  RT::Intersection i3(-3, &s3);
  RT::Sphere s4;
  RT::Intersection i4(2, &s4);
  auto xs = RT::intersections(i1, i2, i3, i4);
  auto i = RT::hit(xs);
  REQUIRE(i.has_value());
  REQUIRE(i.value().first == 2);
  REQUIRE(i.value().second == &s4);
}

TEST_CASE("Translating a ray", "[Ray]") {
  RT::Ray r(RT::point(1, 2, 3), RT::vector(0, 1, 0));
  RT::Transformation m = RT::translation(3, 4, 5);
  auto r2 = r.transform(m);
  REQUIRE(r2.origin == RT::point(4, 6, 8));
  REQUIRE(r2.direction == RT::vector(0, 1, 0));
}

TEST_CASE("Scaling a ray", "[Ray]") {
  RT::Ray r(RT::point(1, 2, 3), RT::vector(0, 1, 0));
  RT::Transformation m = RT::scaling(2, 3, 4);
  auto r2 = r.transform(m);
  REQUIRE(r2.origin == RT::point(2, 6, 12));
  REQUIRE(r2.direction == RT::vector(0, 3, 0));
}

TEST_CASE("A sphere's default transformation", "[Sphere]") {
  RT::Sphere s;
  REQUIRE(s.transformation == RT::identityMatrix<4>());
}
TEST_CASE("Changing a sphere's transformation", "[Sphere]") {
  RT::Sphere s;
  RT::Transformation t = RT::translation(2, 3, 4);
  s.transformation = t;
  REQUIRE(s.transformation == t);
}

TEST_CASE("Intersecting a scaled sphere with a ray", "[Sphere]") {
  RT::Ray r(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  RT::Sphere s;
  s.transformation = RT::scaling(2, 2, 2);
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 3);
  REQUIRE(xs[1].first == 7);
}

TEST_CASE("Intersecting a translated sphere with a ray", "[Sphere]") {
  RT::Ray r(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  RT::Sphere s;
  s.transformation = RT::translation(5, 0, 0);
  auto xs = s.intersect(r);
  REQUIRE(xs.size() == 0);
}
