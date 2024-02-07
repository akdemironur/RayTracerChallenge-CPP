#include "Shape.hpp"
#include "Matrix.hpp"
#include "Pattern.hpp"

namespace RT {

Material::Material()
    : color(RT::color(1, 1, 1)), ambient(0.1), diffuse(0.9), specular(0.9),
      shininess(200) {}

Material::Material(Color color, double ambient, double diffuse, double specular,
                   double shininess)
    : color(color), ambient(ambient), diffuse(diffuse), specular(specular),
      shininess(shininess) {}

Material::Material(const Material &m)
    : color(m.color), ambient(m.ambient), diffuse(m.diffuse),
      specular(m.specular), shininess(m.shininess), pattern() {
  if (m.pattern) {
    pattern = m.pattern->clone();
  }
}

bool Material::operator==(const Material &m) const {
  return color == m.color && isEqual(ambient, m.ambient) &&
         isEqual(diffuse, m.diffuse) && isEqual(specular, m.specular) &&
         isEqual(shininess, m.shininess);
}

bool Material::operator!=(const Material &m) const { return !(*this == m); }

Color Shape::patternAt(const Point &point) const {
  assert(material.pattern != nullptr && "Pattern is null");
  auto objectPoint = transformation.inverse() * point;
  auto patternPoint = material.pattern->transformation.inverse() * objectPoint;

  return material.pattern->patternAt(patternPoint);
}

Vector Sphere::normalAt(const Point &p) const {
  auto objectPoint = transformation.inverse() * p;
  auto objectNormal = objectPoint - point(0, 0, 0);
  auto worldNormal = transformation.inverse().transpose() * objectNormal;
  worldNormal.w = 0;
  return worldNormal.norm();
}

Vector Plane::normalAt(const Point &p) const { return vector(0, 1, 0); }

Tuple lighting(const Material &material, const Light &light, const Point &point,
               const Vector &eye, const Vector &normal, bool inShadow) {
  RT::Color localcolor;
  if (material.pattern) {
    localcolor = material.pattern->patternAt(point);
  } else {
    localcolor = material.color;
  }

  auto effectiveColor = hadamard(localcolor, light.intensity);
  auto ambient = effectiveColor * material.ambient;

  if (inShadow) {
    return ambient;
  }

  auto lightV = (light.position - point).norm();
  auto lightDotNormal = dot(lightV, normal);
  auto diffuse = color(0, 0, 0);
  auto specular = color(0, 0, 0);
  if (lightDotNormal >= 0) {
    diffuse = effectiveColor * material.diffuse * lightDotNormal;
    auto reflectV = (-lightV).reflect(normal);
    auto reflectDotEye = dot(reflectV, eye);
    if (reflectDotEye > 0) {
      auto factor = std::pow(reflectDotEye, material.shininess);
      specular = light.intensity * material.specular * factor;
    }
  }
  return ambient + diffuse + specular;
}

std::vector<Intersection> Sphere::intersect(const Ray &r) const {
  std::vector<Intersection> xs;
  auto ray = r.transform(transformation.inverse());
  auto sphere_to_ray = ray.origin - point(0, 0, 0);
  auto a = dot(ray.direction, ray.direction);
  auto b = 2 * dot(ray.direction, sphere_to_ray);
  auto c = dot(sphere_to_ray, sphere_to_ray) - 1;
  auto discriminant = b * b - 4 * a * c;
  if (discriminant >= 0) {
    xs.push_back(Intersection((-b - sqrt(discriminant)) / (2 * a), this));
    xs.push_back(Intersection((-b + sqrt(discriminant)) / (2 * a), this));
  }
  return xs;
}
std::vector<Intersection> Plane::intersect(const Ray &r) const {
  std::vector<Intersection> xs;
  auto ray = r.transform(transformation.inverse());

  if (std::abs(ray.direction.y) < EPSILON) {
    return xs;
  }

  auto t = -ray.origin.y / ray.direction.y;
  xs.push_back(Intersection(t, this));

  return xs;
}

std::optional<Intersection> hit(const std::vector<Intersection> &xs) {
  std::optional<Intersection> result;
  for (const auto &i : xs) {
    if (i.first >= 0) {
      if (result) {
        if (i.first < result->first) {
          result.emplace(i.first, i.second);
        }
      } else {
        result.emplace(i.first, i.second);
      }
    }
  }
  return result;
}

Computations::Computations(const Intersection &i, const Ray &r) {
  t = i.first;
  object = i.second;
  point = r.position(t);
  eye = -r.direction;
  normal = object->normalAt(point);
  if (dot(eye, normal) < 0) {
    inside = true;
    normal = -normal;
  } else {
    inside = false;
  }
  overPoint = point + normal * EPSILON;
}

Material &Material::operator=(const Material &m) {
  color = m.color;
  ambient = m.ambient;
  diffuse = m.diffuse;
  specular = m.specular;
  shininess = m.shininess;
  if (m.pattern) {
    pattern = m.pattern->clone();
  }
  return *this;
}

Material &Material::operator=(Material &&m) noexcept {
  color = m.color;
  ambient = m.ambient;
  diffuse = m.diffuse;
  specular = m.specular;
  shininess = m.shininess;
  if (m.pattern) {
    pattern = std::move(m.pattern);
  }
  return *this;
}

} // namespace RT
