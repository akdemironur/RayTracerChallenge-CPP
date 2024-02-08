#include "Shape.hpp"
#include "Matrix.hpp"
#include "Pattern.hpp"
#include <istream>
#include <vector>

namespace RT {

Material::Material()
    : color(RT::color(1, 1, 1)), ambient(0.1), diffuse(0.9), specular(0.9),
      shininess(200), reflective(0), transparency(0), refractiveIndex(1.0) {}

Material::Material(Color color, double ambient, double diffuse, double specular,
                   double shininess, double reflectivity, double transparency,
                   double refractiveIndex)
    : color(color), ambient(ambient), diffuse(diffuse), specular(specular),
      shininess(shininess), reflective(reflectivity),
      transparency(transparency), refractiveIndex(refractiveIndex) {}

Material::Material(const Material &m)
    : color(m.color), ambient(m.ambient), diffuse(m.diffuse),
      specular(m.specular), shininess(m.shininess), pattern(),
      reflective(m.reflective), transparency(m.transparency),
      refractiveIndex(m.refractiveIndex) {
  if (m.pattern) {
    pattern = m.pattern->clone();
  }
}

bool Material::operator==(const Material &m) const {
  return color == m.color && isEqual(ambient, m.ambient) &&
         isEqual(diffuse, m.diffuse) && isEqual(specular, m.specular) &&
         isEqual(shininess, m.shininess) && isEqual(reflective, m.reflective) &&
         isEqual(transparency, m.transparency) &&
         isEqual(refractiveIndex, m.refractiveIndex);
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

Tuple Shape::lighting(const Light &light, const Point &point, const Vector &eye,
                      const Vector &normal, bool inShadow) const {
  RT::Color localcolor;
  if (material.pattern) {
    localcolor = patternAt(point);
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

Computations::Computations(const Intersection &i, const Ray &r,
                           const std::vector<Intersection> &xsp) {
  const std::vector<Intersection> &xs = [&]() {
    if (xsp.empty()) {
      return std::vector<Intersection>{i};
    }
    return xsp;
  }();
  std::vector<const Shape *> container;
  auto &h = i;
  for (auto &i : xs) {
    if (i == h) {
      if (container.empty()) {
        n1 = 1;
      } else {
        n1 = container.back()->material.refractiveIndex;
      }
    }
    if (std::find(container.begin(), container.end(), i.second) !=
        container.end()) {
      container.erase(std::remove(container.begin(), container.end(), i.second),
                      container.end());
    } else {
      container.push_back(i.second);
    }

    if (i == h) {
      if (container.empty()) {
        n2 = 1;
      } else {
        n2 = container.back()->material.refractiveIndex;
      }
      break;
    }
  }
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
  underPoint = point - normal * EPSILON;
  reflect = r.direction.reflect(normal);
}

Material &Material::operator=(const Material &m) {
  color = m.color;
  ambient = m.ambient;
  diffuse = m.diffuse;
  specular = m.specular;
  shininess = m.shininess;
  reflective = m.reflective;
  transparency = m.transparency;
  refractiveIndex = m.refractiveIndex;
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
  reflective = m.reflective;
  transparency = m.transparency;
  refractiveIndex = m.refractiveIndex;
  if (m.pattern) {
    pattern = std::move(m.pattern);
  }
  return *this;
}

Sphere glassSphere(Transformation transform, double transparency,
                   double refractiveIndex) {
  auto s = Sphere();
  s.transformation = transform;
  s.material.transparency = transparency;
  s.material.refractiveIndex = refractiveIndex;
  return s;
}

double Computations::schlick() const {
  auto cos = dot(eye, normal);
  if (n1 > n2) {
    auto n = n1 / n2;
    auto sin2_t = n * n * (1 - cos * cos);
    if (sin2_t > 1) {
      return 1.0;
    }
    auto cos_t = std::sqrt(1 - sin2_t);
    cos = cos_t;
  }
  auto r0 = std::pow((n1 - n2) / (n1 + n2), 2);

  return r0 + (1 - r0) * std::pow(1 - cos, 5);
}

} // namespace RT
