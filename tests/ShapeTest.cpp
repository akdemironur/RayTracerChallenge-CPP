#include "Shape.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("The normal on a sphere at a point on the x axis", "[Sphere]") {
  RT::Sphere s;
  auto n = s.normalAt(RT::point(1, 0, 0));
  REQUIRE(n == RT::vector(1, 0, 0));
}

TEST_CASE("The normal on a sphere at a point on the y axis", "[Sphere]") {
  RT::Sphere s;
  auto n = s.normalAt(RT::point(0, 1, 0));
  REQUIRE(n == RT::vector(0, 1, 0));
}

TEST_CASE("The normal on a sphere at a point on the z axis", "[Sphere]") {
  RT::Sphere s;
  auto n = s.normalAt(RT::point(0, 0, 1));
  REQUIRE(n == RT::vector(0, 0, 1));
}

TEST_CASE("The normal on a sphere at a non-axial point", "[Sphere]") {
  RT::Sphere s;
  auto n = s.normalAt(RT::point(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
  REQUIRE(n == RT::vector(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
}

TEST_CASE("The normal is a normalized vector", "[Sphere]") {
  RT::Sphere s;
  auto n = s.normalAt(RT::point(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
  REQUIRE(n == n.norm());
}

TEST_CASE("Computing the normal on a translated sphere", "[Sphere]") {
  RT::Sphere s;
  s.transformation = RT::translation(0, 1, 0);
  auto n = s.normalAt(RT::point(0, 1.70711, -0.70711));
  REQUIRE(n == RT::vector(0, 0.70711, -0.70711));
}

TEST_CASE("Computing the normal on a transformed sphere", "[Sphere]") {
  RT::Sphere s;
  s.transformation = RT::scaling(1, 0.5, 1) * RT::rotationZ(M_PI / 5);
  auto n = s.normalAt(RT::point(0, sqrt(2) / 2, -sqrt(2) / 2));
  REQUIRE(n == RT::vector(0, 0.97014, -0.24254));
}

TEST_CASE("The default material", "[Material]") {
  auto m = RT::Material();
  REQUIRE(m.color == RT::color(1, 1, 1));
  REQUIRE(m.ambient == 0.1);
  REQUIRE(m.diffuse == 0.9);
  REQUIRE(m.specular == 0.9);
  REQUIRE(m.shininess == 200);
}

TEST_CASE("A sphere has a default material", "[Sphere]") {
  RT::Sphere s;
  auto m = s.material;
  REQUIRE(m == RT::Material());
}

TEST_CASE("A sphere may be assigned a material", "[Sphere]") {
  RT::Sphere s;
  RT::Material m;
  m.ambient = 1;
  s.material = m;
  REQUIRE(s.material == m);
}

TEST_CASE("Lighting with the eye between the light and the surface",
          "[Material]") {
  RT::Material m;
  RT::Point position = RT::point(0, 0, 0);
  RT::Vector eye = RT::vector(0, 0, -1);
  RT::Vector normal = RT::vector(0, 0, -1);
  RT::Light light = RT::Light(RT::point(0, 0, -10), RT::color(1, 1, 1));
  RT::Sphere s(RT::identityMatrix<4>(), m);
  auto result = s.lighting(light, position, eye, normal);
  REQUIRE(result == RT::color(1.9, 1.9, 1.9));
}

TEST_CASE(
    "Lighting with the eye between the light and the surface, eye offset 45",
    "[Material]") {
  RT::Material m;
  RT::Point position = RT::point(0, 0, 0);
  RT::Vector eye = RT::vector(0, sqrt(2) / 2, -sqrt(2) / 2);
  RT::Vector normal = RT::vector(0, 0, -1);
  RT::Light light = RT::Light(RT::point(0, 0, -10), RT::color(1, 1, 1));
  RT::Sphere s(RT::identityMatrix<4>(), m);
  auto result = s.lighting(light, position, eye, normal);
  REQUIRE(result == RT::color(1.0, 1.0, 1.0));
}

TEST_CASE("Lighting with the eye opposite the surface, light offset 45",
          "[Material]") {
  RT::Material m;
  RT::Point position = RT::point(0, 0, 0);
  RT::Vector eye = RT::vector(0, 0, -1);
  RT::Vector normal = RT::vector(0, 0, -1);
  RT::Light light = RT::Light(RT::point(0, 10, -10), RT::color(1, 1, 1));
  RT::Sphere s(RT::identityMatrix<4>(), m);
  auto result = s.lighting(light, position, eye, normal);
  REQUIRE(result == RT::color(0.7364, 0.7364, 0.7364));
}

TEST_CASE("Lighting with the eye in the path of the reflection vector",
          "[Material]") {
  RT::Material m;
  RT::Point position = RT::point(0, 0, 0);
  RT::Vector eye = RT::vector(0, -sqrt(2) / 2, -sqrt(2) / 2);
  RT::Vector normal = RT::vector(0, 0, -1);
  RT::Light light = RT::Light(RT::point(0, 10, -10), RT::color(1, 1, 1));
  RT::Sphere s(RT::identityMatrix<4>(), m);
  auto result = s.lighting(light, position, eye, normal);
  REQUIRE(result == RT::color(1.6364, 1.6364, 1.6364));
}

TEST_CASE("Lighting with the light behind the surface", "[Material]") {
  RT::Material m;
  RT::Point position = RT::point(0, 0, 0);
  RT::Vector eye = RT::vector(0, 0, -1);
  RT::Vector normal = RT::vector(0, 0, -1);
  RT::Light light = RT::Light(RT::point(0, 0, 10), RT::color(1, 1, 1));
  RT::Sphere s(RT::identityMatrix<4>(), m);
  auto result = s.lighting(light, position, eye, normal);
  REQUIRE(result == RT::color(0.1, 0.1, 0.1));
}

TEST_CASE("Precomputing the state of an intersection", "[Intersection]") {
  RT::Sphere s;
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto i = RT::Intersection(4, &s);
  auto comps = RT::Computations(i, r);
  REQUIRE(comps.t == i.first);
  REQUIRE(comps.object == i.second);
  REQUIRE(comps.point == RT::point(0, 0, -1));
  REQUIRE(comps.eye == RT::vector(0, 0, -1));
  REQUIRE(comps.normal == RT::vector(0, 0, -1));
}

TEST_CASE("The hit, when an intersection occurs on the outside",
          "[Intersection]") {
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto s = RT::Sphere();
  auto i = RT::Intersection(4, &s);
  auto comps = RT::Computations(i, r);
  REQUIRE(!comps.inside);
}

TEST_CASE("The hit, when an intersection occurs on the inside",
          "[Intersection]") {
  auto r = RT::Ray(RT::point(0, 0, 0), RT::vector(0, 0, 1));
  auto s = RT::Sphere();
  auto i = RT::Intersection(1, &s);
  auto comps = RT::Computations(i, r);
  REQUIRE(comps.point == RT::point(0, 0, 1));
  REQUIRE(comps.eye == RT::vector(0, 0, -1));
  REQUIRE(comps.normal == RT::vector(0, 0, -1));
  REQUIRE(comps.inside);
}

TEST_CASE("Lighting with the surface in shadow", "[Intersection]") {
  RT::Material m;
  RT::Point position = RT::point(0, 0, 0);
  RT::Vector eye = RT::vector(0, 0, -1);
  RT::Vector normal = RT::vector(0, 0, -1);
  RT::Light light = RT::Light(RT::point(0, 0, -10), RT::color(1, 1, 1));
  RT::Sphere s(RT::identityMatrix<4>(), m);
  auto i = RT::Intersection(1, &s);
  bool inShadow = true;
  auto result = s.lighting(light, position, eye, normal, inShadow);
  REQUIRE(result == RT::color(0.1, 0.1, 0.1));
}

TEST_CASE("The hit should offset the point", "[Intersection]") {
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto s = RT::Sphere(); // Replace with a concrete subclass of RT::Shape
  s.transformation = RT::translation(0, 0, 1);
  auto i = RT::Intersection(5, &s);
  auto comps = RT::Computations(i, r);
  REQUIRE(comps.overPoint.z < -EPSILON / 2);
  REQUIRE(comps.point.z > comps.overPoint.z);
}

TEST_CASE("The normal of a plane is constant everywhere", "[Plane]") {
  RT::Plane p;
  auto n1 = p.normalAt(RT::point(0, 0, 0));
  auto n2 = p.normalAt(RT::point(10, 0, -10));
  auto n3 = p.normalAt(RT::point(-5, 0, 150));
  REQUIRE(n1 == RT::vector(0, 1, 0));
  REQUIRE(n2 == RT::vector(0, 1, 0));
  REQUIRE(n3 == RT::vector(0, 1, 0));
}

TEST_CASE("Intersect with a ray parallel to the plane", "[Plane]") {
  RT::Plane p;
  auto r = RT::Ray(RT::point(0, 10, 0), RT::vector(0, 0, 1));
  auto xs = p.intersect(r);
  REQUIRE(xs.size() == 0);
}

TEST_CASE("Intersect with a coplanar ray", "[Plane]") {
  RT::Plane p;
  auto r = RT::Ray(RT::point(0, 0, 0), RT::vector(0, 0, 1));
  auto xs = p.intersect(r);
  REQUIRE(xs.size() == 0);
}

TEST_CASE("A ray intersecting a plane from above", "[Plane]") {
  RT::Plane p;
  auto r = RT::Ray(RT::point(0, 1, 0), RT::vector(0, -1, 0));
  auto xs = p.intersect(r);
  REQUIRE(xs.size() == 1);
  REQUIRE(xs[0].first == 1);
  REQUIRE(xs[0].second == &p);
}

TEST_CASE("A ray intersecting a plane from below", "[Plane]") {
  RT::Plane p;
  auto r = RT::Ray(RT::point(0, -1, 0), RT::vector(0, 1, 0));
  auto xs = p.intersect(r);
  REQUIRE(xs.size() == 1);
  REQUIRE(xs[0].first == 1);
  REQUIRE(xs[0].second == &p);
}

TEST_CASE("Reflectivity for the default material", "[Material]") {
  RT::Material m;
  REQUIRE(m.reflective == 0.0);
}

TEST_CASE("Precomputing the reflection vector", "[Intersection]") {
  RT::Plane p;
  auto r =
      RT::Ray(RT::point(0, 1, -1), RT::vector(0, -sqrt(2) / 2, sqrt(2) / 2));
  auto i = RT::Intersection(sqrt(2), &p);
  auto comps = RT::Computations(i, r);
  REQUIRE(comps.reflect == RT::vector(0, sqrt(2) / 2, sqrt(2) / 2));
}

TEST_CASE("Transparency and Refractive Index for the default material",
          "[Material]") {

  auto m = RT::Material();
  REQUIRE(m.transparency == 0);
  REQUIRE(m.refractiveIndex == 1);
}

TEST_CASE("A helper for producing a glassy sphere", "[Sphere]") {
  auto s = RT::glassSphere();
  REQUIRE(s.transformation == RT::identityMatrix<4>());
  REQUIRE(s.material.transparency == 1.0);
  REQUIRE(s.material.refractiveIndex == 1.5);
}

TEST_CASE("Finding n1 and n2 at various intersections", "[Intersection]") {
  auto a = RT::glassSphere(RT::scaling(2, 2, 2));
  auto b = RT::glassSphere(RT::translation(0, 0, -0.25), 1, 2.0);
  auto c = RT::glassSphere(RT::translation(0, 0, 0.25), 1, 2.5);
  auto r = RT::Ray(RT::point(0, 0, -4), RT::vector(0, 0, 1));
  auto xs =
      RT::intersections(RT::Intersection(2, &a), RT::Intersection(2.75, &b),
                        RT::Intersection(3.25, &c), RT::Intersection(4.75, &b),
                        RT::Intersection(5.25, &c), RT::Intersection(6, &a));

  auto n1s = std::vector<double>{1.0, 1.5, 2.0, 2.5, 2.5, 1.5};
  auto n2s = std::vector<double>{1.5, 2.0, 2.5, 2.5, 1.5, 1.0};

  for (auto i = 0; i < xs.size(); i++) {
    auto comps = RT::Computations(xs[i], r, xs);
    REQUIRE(comps.n1 == n1s[i]);
    REQUIRE(comps.n2 == n2s[i]);
  }
}

TEST_CASE("The under point is offset below the surface", "[Intersection]") {
  auto r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto s = RT::glassSphere(RT::translation(0, 0, 1));
  auto i = RT::Intersection(5, &s);
  auto xs = RT::intersections(i);
  auto comps = RT::Computations(i, r, xs);
  REQUIRE(comps.underPoint.z > EPSILON / 2);
  REQUIRE(comps.point.z < comps.underPoint.z);
}