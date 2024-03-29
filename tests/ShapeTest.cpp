#include "Shape.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "Util.hpp"
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

TEST_CASE("The Schlick approximation under total internal reflection",
          "[Intersection]") {
  auto s = RT::glassSphere();
  auto r = RT::Ray(RT::point(0, 0, sqrt(2) / 2), RT::vector(0, 1, 0));
  auto xs = RT::intersections(RT::Intersection(-sqrt(2) / 2, &s),
                              RT::Intersection(sqrt(2) / 2, &s));
  auto comps = RT::Computations(xs[1], r, xs);
  auto reflectance = comps.schlick();
  REQUIRE(reflectance == 1.0);
}

TEST_CASE("The Schlick approximation with a perpendicular viewing angle",
          "[Intersection]") {
  auto s = RT::glassSphere();
  auto r = RT::Ray(RT::point(0, 0, 0), RT::vector(0, 1, 0));
  auto xs =
      RT::intersections(RT::Intersection(-1, &s), RT::Intersection(1, &s));
  auto comps = RT::Computations(xs[1], r, xs);
  auto reflectance = comps.schlick();
  REQUIRE(RT::approxEqual(reflectance, 0.04));
}

TEST_CASE("The Schlick approximation with small angle and n2 > n1",
          "[Intersection]") {
  auto s = RT::glassSphere();
  auto r = RT::Ray(RT::point(0, 0.99, -2), RT::vector(0, 0, 1));
  auto xs = RT::intersections(RT::Intersection(1.8589, &s));
  auto comps = RT::Computations(xs[0], r, xs);
  auto reflectance = comps.schlick();
  REQUIRE(RT::approxEqual(reflectance, 0.48873));
}
TEST_CASE("A ray intersects a cube") {
  RT::Cube c;
  auto r = RT::Ray(RT::point(5, 0.5, 0), RT::vector(-1, 0, 0));
  auto xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 4);
  REQUIRE(xs[1].first == 6);

  r = RT::Ray(RT::point(-5, 0.5, 0), RT::vector(1, 0, 0));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 4);
  REQUIRE(xs[1].first == 6);

  r = RT::Ray(RT::point(0.5, 5, 0), RT::vector(0, -1, 0));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 4);
  REQUIRE(xs[1].first == 6);

  r = RT::Ray(RT::point(0.5, -5, 0), RT::vector(0, 1, 0));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 4);
  REQUIRE(xs[1].first == 6);

  r = RT::Ray(RT::point(0.5, 0, 5), RT::vector(0, 0, -1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 4);
  REQUIRE(xs[1].first == 6);

  r = RT::Ray(RT::point(0.5, 0, -5), RT::vector(0, 0, 1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == 4);
  REQUIRE(xs[1].first == 6);

  r = RT::Ray(RT::point(0, 0.5, 0), RT::vector(0, 0, 1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(xs[0].first == -1);
  REQUIRE(xs[1].first == 1);
}

TEST_CASE("A ray misses a cube") {
  RT::Cube c;
  auto r = RT::Ray(RT::point(-2, 0, 0), RT::vector(0.2673, 0.5345, 0.8018));
  auto xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, -2, 0), RT::vector(0.8018, 0.2673, 0.5345));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 0, -2), RT::vector(0.5345, 0.8018, 0.2673));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(2, 0, 2), RT::vector(0, 0, -1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 2, 2), RT::vector(0, -1, 0));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(2, 2, 0), RT::vector(-1, 0, 0));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);
}

TEST_CASE("The normal on the surface of a cube") {
  auto c = RT::Cube();
  auto p = RT::point(1, 0.5, -0.8);
  auto n = c.normalAt(p);
  REQUIRE(n == RT::vector(1, 0, 0));

  p = RT::point(-1, -0.2, 0.9);
  n = c.normalAt(p);
  REQUIRE(n == RT::vector(-1, 0, 0));

  p = RT::point(-0.4, 1, -0.1);
  n = c.normalAt(p);
  REQUIRE(n == RT::vector(0, 1, 0));

  p = RT::point(0.3, -1, -0.7);
  n = c.normalAt(p);
  REQUIRE(n == RT::vector(0, -1, 0));

  p = RT::point(-0.6, 0.3, 1);
  n = c.normalAt(p);
  REQUIRE(n == RT::vector(0, 0, 1));

  p = RT::point(0.4, 0.4, -1);
  n = c.normalAt(p);
  REQUIRE(n == RT::vector(0, 0, -1));

  p = RT::point(1, 1, 1);
  n = c.normalAt(p);
  REQUIRE(n == RT::vector(1, 0, 0));

  p = RT::point(-1, -1, -1);
  n = c.normalAt(p);
  REQUIRE(n == RT::vector(-1, 0, 0));
}

TEST_CASE("A ray misses a cylinder", "[Cylinder]") {
  RT::Ray r(RT::point(1, 0, 0), RT::vector(0, 1, 0));
  RT::Cylinder c;
  auto xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 0, 0), RT::vector(0, 1, 0));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 0, -5), RT::vector(1, 1, 1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);
}

TEST_CASE("A ray strikes a cylinder", "[Cylinder]") {
  RT::Ray r(RT::point(1, 0, -5), RT::vector(0, 0, 1));
  RT::Cylinder c;
  auto xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(RT::approxEqual(xs[0].first, 5.0));
  REQUIRE(RT::approxEqual(xs[1].first, 5.0));

  r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(RT::approxEqual(xs[0].first, 4.0));
  REQUIRE(RT::approxEqual(xs[1].first, 6.0));

  r = RT::Ray(RT::point(0.5, 0, -5), RT::vector(0.1, 1, 1).norm());
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(RT::approxEqual(xs[0].first, 6.80798));
  REQUIRE(RT::approxEqual(xs[1].first, 7.08872));
}

TEST_CASE("Normal vector on a cylinder", "[Cylinder]") {
  RT::Cylinder c;
  auto n = c.normalAt(RT::point(1, 0, 0));
  REQUIRE(n == RT::vector(1, 0, 0));

  n = c.normalAt(RT::point(0, 5, -1));
  REQUIRE(n == RT::vector(0, 0, -1));

  n = c.normalAt(RT::point(0, -2, 1));
  REQUIRE(n == RT::vector(0, 0, 1));

  n = c.normalAt(RT::point(-1, 1, 0));
  REQUIRE(n == RT::vector(-1, 0, 0));
}

TEST_CASE("The default minimum and maximum for a cylinder", "[Cylinder]") {
  RT::Cylinder c;
  REQUIRE(c.minimum == -INFINITY);
  REQUIRE(c.maximum == INFINITY);
}

TEST_CASE("Intersecting a constrained cylinder", "[Cylinder]") {
  RT::Cylinder c;
  c.minimum = 1;
  c.maximum = 2;
  RT::Ray r(RT::point(0, 1.5, -5), RT::vector(0.1, 1, 0).norm());
  auto xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 3, -5), RT::vector(0, 0, 1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 2, -5), RT::vector(0, 0, 1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 1, -5), RT::vector(0, 0, 1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 1.5, -2), RT::vector(0, 0, 1));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
}

TEST_CASE("The default closed value for a cylinder", "[Cylinder]") {
  RT::Cylinder c;
  REQUIRE(c.closed == false);
}
TEST_CASE("Intersecting the caps of a closed cylinder", "[Cylinder]") {
  RT::Cylinder c;
  c.minimum = 1;
  c.maximum = 2;
  c.closed = true;
  RT::Ray r(RT::point(0, 3, 0), RT::vector(0, -1, 0));
  auto xs = c.localIntersect(r);
  REQUIRE(xs.size() == 2);

  r = RT::Ray(RT::point(0, 3, -2), RT::vector(0, -1, 2).norm());
  xs = c.localIntersect(r);
  REQUIRE(xs.size() == 2);

  r = RT::Ray(RT::point(0, 4, -2), RT::vector(0, -1, 1).norm());
  xs = c.localIntersect(r);
  REQUIRE(xs.size() == 2);

  r = RT::Ray(RT::point(0, 0, -2), RT::vector(0, 1, 2).norm());
  xs = c.localIntersect(r);
  REQUIRE(xs.size() == 2);

  r = RT::Ray(RT::point(0, -1, -2), RT::vector(0, 1, 1).norm());
  xs = c.localIntersect(r);
  REQUIRE(xs.size() == 2);
}

TEST_CASE("The normal vector on a cylinder's end caps") {
  auto c = RT::Cylinder();
  c.minimum = 1;
  c.maximum = 2;
  c.closed = true;
  auto n = c.localNormalAt(RT::point(0, 1, 0));
  REQUIRE(n == RT::vector(0, -1, 0));

  n = c.localNormalAt(RT::point(0.5, 1, 0));
  REQUIRE(n == RT::vector(0, -1, 0));

  n = c.localNormalAt(RT::point(0, 1, 0.5));
  REQUIRE(n == RT::vector(0, -1, 0));

  n = c.localNormalAt(RT::point(0, 2, 0));
  REQUIRE(n == RT::vector(0, 1, 0));

  n = c.localNormalAt(RT::point(0.5, 2, 0));
  REQUIRE(n == RT::vector(0, 1, 0));

  n = c.localNormalAt(RT::point(0, 2, 0.5));
  REQUIRE(n == RT::vector(0, 1, 0));
}

TEST_CASE("Intersecting a cone with a ray") {
  RT::Cone c;
  RT::Ray r(RT::point(0, 0, -5), RT::vector(0, 0, 1));
  auto xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(RT::approxEqual(xs[0].first, 5.0));
  REQUIRE(RT::approxEqual(xs[1].first, 5.0));

  r = RT::Ray(RT::point(0, 0, -5), RT::vector(1, 1, 1).norm());
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(RT::approxEqual(xs[0].first, 8.66025));
  REQUIRE(RT::approxEqual(xs[1].first, 8.66025));

  r = RT::Ray(RT::point(1, 1, -5), RT::vector(-0.5, -1, 1).norm());
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);
  REQUIRE(RT::approxEqual(xs[0].first, 4.55006));
  REQUIRE(RT::approxEqual(xs[1].first, 49.44994));
}

TEST_CASE("Intersecting a cone with a ray parallel to one of its halves") {
  RT::Cone c;
  RT::Ray r(RT::point(0, 0, -1), RT::vector(0, 1, 1).norm());
  auto xs = c.intersect(r);
  REQUIRE(xs.size() == 1);
  REQUIRE(RT::approxEqual(xs[0].first, 0.35355));
}

TEST_CASE("Intersecting a cone's end caps") {
  RT::Cone c;
  c.minimum = -0.5;
  c.maximum = 0.5;
  c.closed = true;
  RT::Ray r(RT::point(0, 0, -5), RT::vector(0, 1, 0));
  auto xs = c.intersect(r);
  REQUIRE(xs.size() == 0);

  r = RT::Ray(RT::point(0, 0, -0.25), RT::vector(0, 1, 1).norm());
  xs = c.intersect(r);
  REQUIRE(xs.size() == 2);

  r = RT::Ray(RT::point(0, 0, -0.25), RT::vector(0, 1, 0));
  xs = c.intersect(r);
  REQUIRE(xs.size() == 4);
}