#include "World.hpp"
#include "Matrix.hpp"
#include "Shape.hpp"
#include "Util.hpp"
#include <memory>

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
  for (const std::unique_ptr<Shape> &object : objects) {
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

Color World::reflectedColor(const Computations &comps, int remaining) const {
  if (isEqual(comps.object->material.reflective, 0.0) || remaining <= 0) {
    return color(0, 0, 0);
  }
  auto reflectRay = Ray(comps.overPoint, comps.reflect);
  auto color = colorAt(reflectRay, remaining - 1);
  return color * comps.object->material.reflective;
}

Color World::refractedColor(const Computations &comps, int remaining) const {
  if (isEqual(comps.object->material.transparency, 0.0) || remaining <= 0) {
    return color(0, 0, 0);
  }
  auto nRatio = comps.n1 / comps.n2;
  auto cosI = dot(comps.eye, comps.normal);
  auto sin2T = nRatio * nRatio * (1 - cosI * cosI);
  if (sin2T > 1) {
    return color(0, 0, 0);
  }
  auto cosT = std::sqrt(1.0 - sin2T);
  auto direction = comps.normal * (nRatio * cosI - cosT) - comps.eye * nRatio;
  auto refractRay = Ray(comps.underPoint, direction);
  return colorAt(refractRay, remaining - 1) *
         comps.object->material.transparency;
}

Color World::shadeHit(const Computations &comps, int remaining) const {
  bool isShadowed = this->isShadowed(comps.overPoint);
  auto surface = comps.object->lighting(light, comps.overPoint, comps.eye,
                                        comps.normal, isShadowed);
  auto reflected = reflectedColor(comps, remaining);
  auto refracted = refractedColor(comps, remaining);

  auto material = comps.object->material;
  if (material.reflective != 0 && material.transparency != 0) {
    auto reflectance = comps.schlick();
    return surface + reflected * reflectance + refracted * (1 - reflectance);
  }

  return refracted + reflected + surface;
}

Color World::colorAt(const Ray &ray, int remaining) const {
  auto xs = intersect(ray);
  auto i = hit(xs);
  if (i.has_value()) {
    auto comps = Computations(i.value(), ray, xs);
    return shadeHit(comps, remaining);
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