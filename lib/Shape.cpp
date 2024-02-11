#include "Shape.hpp"

#include "Matrix.hpp"
#include "Pattern.hpp"
#include <cmath>
#include <istream>
#include <utility>
#include <vector>

namespace RT {

const double DEFAULT_AMBIENT = 0.1;
const double DEFAULT_DIFFUSE = 0.9;
const double DEFAULT_SPECULAR = 0.9;
const double DEFAULT_SHININESS = 200;

Material::Material()
    : color(RT::color(1, 1, 1)), ambient(DEFAULT_AMBIENT),
      diffuse(DEFAULT_DIFFUSE), specular(DEFAULT_SPECULAR),
      shininess(DEFAULT_SHININESS), reflective(0), transparency(0),
      refractiveIndex(1.0) {}

Material::Material(Color color, double ambient, double diffuse, double specular,
                   double shininess, double reflective, double transparency,
                   double refractiveIndex)
    : color(std::move(color)), ambient(ambient), diffuse(diffuse),
      specular(specular), shininess(shininess), reflective(reflective),
      transparency(transparency), refractiveIndex(refractiveIndex) {}

Material::Material(const Material &material)
    : color(material.color), ambient(material.ambient),
      diffuse(material.diffuse), specular(material.specular),
      shininess(material.shininess), reflective(material.reflective),
      transparency(material.transparency),
      refractiveIndex(material.refractiveIndex) {
  if (material.pattern) {
    pattern = material.pattern->clone();
  }
}

auto Material::operator==(const Material &m) const -> bool {
  return color == m.color && approxEqual(ambient, m.ambient) &&
         approxEqual(diffuse, m.diffuse) && approxEqual(specular, m.specular) &&
         approxEqual(shininess, m.shininess) &&
         approxEqual(reflective, m.reflective) &&
         approxEqual(transparency, m.transparency) &&
         approxEqual(refractiveIndex, m.refractiveIndex);
}

auto Material::operator!=(const Material &m) const -> bool {
  return !(*this == m);
}

auto Shape::patternAt(const Point &point) const -> Color {
  assert(material.pattern != nullptr && "Pattern is null");
  auto objectPoint = transformation.inverse() * point;
  auto patternPoint = material.pattern->transformation.inverse() * objectPoint;

  return material.pattern->patternAt(patternPoint);
}

auto Shape::normalAt(const Point &point) const -> Vector {
  auto objectPoint = transformation.inverse() * point;
  auto objectNormal = localNormalAt(objectPoint);
  auto worldNormal = transformation.inverse().transpose() * objectNormal;
  worldNormal.w = 0;
  return worldNormal.norm();
}

std::vector<std::pair<double, const Shape *>>
Shape::intersect(const Ray &ray) const {
  auto localRay = ray.transform(transformation.inverse());
  return localIntersect(localRay);
}

Vector Sphere::localNormalAt(const Point &p) const {
  return (p - point(0, 0, 0));
}

Vector Plane::localNormalAt(const Point & /*p*/) const {
  return vector(0, 1, 0);
}

auto Shape::lighting(const Light &light, const Point &point, const Vector &eye,
                     const Vector &normal, bool inShadow) const -> Tuple {
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

std::vector<Intersection> Sphere::localIntersect(const Ray &ray) const {
  std::vector<Intersection> xs;
  auto sphere_to_ray = ray.origin - point(0, 0, 0);
  auto a = dot(ray.direction, ray.direction);
  auto b = 2 * dot(ray.direction, sphere_to_ray);
  auto c = dot(sphere_to_ray, sphere_to_ray) - 1;
  auto discriminant = b * b - 4 * a * c;
  if (discriminant >= 0) {
    xs.emplace_back((-b - sqrt(discriminant)) / (2 * a), this);
    xs.emplace_back((-b + sqrt(discriminant)) / (2 * a), this);
  }
  return xs;
}
std::vector<Intersection> Plane::localIntersect(const Ray &ray) const {
  if (std::abs(ray.direction.y) < EPSILON) {
    return {};
  }
  auto t = -ray.origin.y / ray.direction.y;
  return {Intersection(t, this)};
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
                           const std::vector<Intersection> &xsp)
    : t(i.first), object(i.second) {
  const std::vector<Intersection> &xs = [&]() {
    if (xsp.empty()) {
      return std::vector<Intersection>{i};
    }
    return xsp;
  }();
  std::vector<const Shape *> container;
  const auto &h = i;
  for (const auto &i : xs) {
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
  if (this == &m) {
    return *this;
  }
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

Vector Cube::localNormalAt(const Point &p) const {
  auto maxc = std::max({std::abs(p.x), std::abs(p.y), std::abs(p.z)});

  if (approxEqual(maxc, std::abs(p.x))) {
    return vector(p.x, 0, 0);
  }

  if (approxEqual(maxc, std::abs(p.y))) {
    return vector(0, p.y, 0);
  }
  return vector(0, 0, p.z);
}

std::pair<double, double> checkAxis(double origin, double direction) {
  auto tmin_numerator = -1 - origin;
  auto tmax_numerator = 1 - origin;
  double tmin = NAN;
  double tmax = NAN;
  if (std::abs(direction) >= EPSILON) {
    tmin = tmin_numerator / direction;
    tmax = tmax_numerator / direction;
  } else {
    tmin = tmin_numerator * INFINITY;
    tmax = tmax_numerator * INFINITY;
  }
  if (tmin > tmax) {
    std::swap(tmin, tmax);
  }
  return {tmin, tmax};
}

std::vector<std::pair<double, const Shape *>>
Cube::localIntersect(const Ray &ray) const {
  auto [xtmin, xtmax] = checkAxis(ray.origin.x, ray.direction.x);
  auto [ytmin, ytmax] = checkAxis(ray.origin.y, ray.direction.y);
  auto [ztmin, ztmax] = checkAxis(ray.origin.z, ray.direction.z);
  auto tmin = std::max({xtmin, ytmin, ztmin});
  auto tmax = std::min({xtmax, ytmax, ztmax});
  if (tmin > tmax) {
    return {};
  }
  return {{tmin, this}, {tmax, this}};
}

Vector Cylinder::localNormalAt(const Point &p) const {
  auto dist = p.x * p.x + p.z * p.z;
  if (dist < 1 && p.y >= 1 - EPSILON) {
    return vector(0, 1, 0);
  }
  if (dist < 1 && p.y <= EPSILON) {
    return vector(0, -1, 0);
  }
  return vector(p.x, 0, p.z);
}

bool Cylinder::checkCap(const Ray &ray, double t) {
  auto x = ray.origin.x + t * ray.direction.x;
  auto z = ray.origin.z + t * ray.direction.z;
  return x * x + z * z <= 1;
}

std::vector<std::pair<double, const Shape *>>
Cylinder::intersectCaps(const Ray &ray) const {
  std::vector<std::pair<double, const Shape *>> xs;
  if (!closed || approxEqual(ray.direction.y, 0.0)) {
    return xs;
  }
  auto t = (minimum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t)) {
    xs.emplace_back(t, this);
  }
  t = (maximum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t)) {
    xs.emplace_back(t, this);
  }
  return xs;
}

std::vector<std::pair<double, const Shape *>>
Cylinder::localIntersect(const Ray &ray) const {
  auto a =
      ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
  if (approxEqual(a, 0.0)) {
    std::cout << "a == 0" << std::endl;
    return {};
  }
  auto b =
      2 * ray.origin.x * ray.direction.x + 2 * ray.origin.z * ray.direction.z;
  auto c = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
  auto discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    std::cout << "discriminant < 0" << std::endl;
    return {};
  }

  std::vector<std::pair<double, const Shape *>> xs = intersectCaps(ray);

  auto t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  auto t2 = (-b + std::sqrt(discriminant)) / (2 * a);

  if (t1 > t2) {
    std::swap(t1, t2);
  }

  auto y1 = ray.origin.y + t1 * ray.direction.y;
  if (minimum < y1 && y1 < maximum) {
    xs.emplace_back(t1, this);
  }
  auto y2 = ray.origin.y + t2 * ray.direction.y;
  if (minimum < y2 && y2 < maximum) {
    xs.emplace_back(t2, this);
  }
  return xs;
}

} // namespace RT
