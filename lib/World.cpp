#include "World.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"

namespace RT {

World::World(bool defaultWorld)
    : light{point(-10, 10, -10), color(1, 1, 1)}, objects{} {

  if (defaultWorld) {
    objects.push_back(std::make_unique<Sphere>(Sphere()));
    objects.push_back(std::make_unique<Sphere>(Sphere()));

    objects[0]->material.color = color(0.8, 1.0, 0.6);
    objects[0]->material.diffuse = 0.7;
    objects[0]->material.specular = 0.2;
    objects[1]->transformation = scaling(0.5, 0.5, 0.5);
  }
}

bool World::contains(const Shape &object) const {
  return std::find_if(objects.begin(), objects.end(), [&](const auto &obj) {
           return obj.get()->transformation == object.transformation &&
                  obj.get()->material == object.material;
         }) != objects.end();
}

void World::add(std::unique_ptr<Shape> object) {
  objects.push_back(std::move(object));
}

int World::count() const { return objects.size(); }

std::vector<Intersection> World::intersect(const Ray &ray) const {
  std::vector<Intersection> result;
  for (const auto &object : objects) {
    auto xs = object->intersect(ray);
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