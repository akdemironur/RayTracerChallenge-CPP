#include "World.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"

namespace RT {

World::World(bool defaultWorld)
    : light{point(-10, 10, -10), color(1, 1, 1)}, objects{} {

  if (defaultWorld) {
    objects.push_back(Sphere());
    objects.push_back(Sphere());

    objects[0].material.color = color(0.8, 1.0, 0.6);
    objects[0].material.diffuse = 0.7;
    objects[0].material.specular = 0.2;
    objects[1].transformation = scaling(0.5, 0.5, 0.5);
  }
}

bool World::contains(const Sphere &object) const {
  return std::find(objects.begin(), objects.end(), object) != objects.end();
}

void World::add(const Sphere &object) { objects.push_back(object); }

int World::count() const { return objects.size(); }

std::vector<Intersection> World::intersect(const Ray &ray) const {
  std::vector<Intersection> result;
  for (const auto &object : objects) {
    auto xs = RT::intersect(object, ray);
    for (auto &x : xs) {
      result.push_back(x);
    }
  }

  std::sort(result.begin(), result.end(),
            [](const Intersection &a, const Intersection &b) {
              return a.first < b.first;
            });

  return result;
}

Color World::shadeHit(const Computations &comps, int remaining) const {

  bool isShadowed = this->isShadowed(comps.overPoint);

  return lighting(comps.object->material, light, comps.point, comps.eye,
                  comps.normal, isShadowed);
}

Color World::colorAt(const Ray &ray) const {
  auto xs = intersect(ray);
  auto i = hit(xs);
  if (i.has_value()) {
    auto comps = Computations(i.value(), ray);
    return shadeHit(comps);
  }
  return color(0, 0, 0);
}
bool World::isShadowed(const Point &point) const {
  auto v = light.position - point;
  auto distance = v.magnitude();
  auto direction = v.norm();
  auto r = Ray(point, direction);
  auto xs = intersect(r);
  auto h = hit(xs);
  if (h.has_value() && h.value().first < distance) {
    return true;
  }
  return false;
}
} // namespace RT