#pragma once
#include "Light.hpp"
#include "Matrix.hpp"
#include "Pattern.hpp"
#include "Ray.hpp"
#include "Tuple.hpp"
#include "Util.hpp"
#include <memory>
#include <optional>
#include <utility>
#include <variant>
namespace RT {

class Material {
public:
  Material();
  Material(Color color, double ambient, double diffuse, double specular,
           double shininess, double reflective, double transparency,
           double refractiveIndex);
  Material(const Material &m);
  Color color;
  double ambient;
  double diffuse;
  double specular;
  double shininess;
  double reflective;
  double transparency;
  double refractiveIndex;
  std::unique_ptr<Pattern> pattern;
  bool operator==(const Material &m) const;
  bool operator!=(const Material &m) const;
  Material &operator=(const Material &m);
  Material &operator=(Material &&m) noexcept;
};

class Shape {
public:
  Shape() : transformation(identityMatrix<4>()), material(Material()){};
  Shape(const Transformation &transformation, const Material &material)
      : transformation(transformation), material(material){};
  Transformation transformation;
  Material material;
  Tuple lighting(const Light &light, const Point &point, const Vector &eye,
                 const Vector &normal, bool inShadow = false) const;
  Color patternAt(const Point &point) const;
  virtual Vector localNormalAt(const Point &point) const = 0;
  Vector normalAt(const Point &point) const;
  virtual std::vector<std::pair<double, const Shape *>>
  localIntersect(const Ray &ray) const = 0;
  std::vector<std::pair<double, const Shape *>> intersect(const Ray &ray) const;
  virtual ~Shape() = default;
};

class Sphere : public Shape {
public:
  Sphere() : Shape(){};
  Sphere(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Vector localNormalAt(const Point &point) const override;
  std::vector<std::pair<double, const Shape *>>
  localIntersect(const Ray &ray) const override;
  ~Sphere() = default;
};

class Plane : public Shape {
public:
  Plane() : Shape(){};
  Plane(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Vector localNormalAt(const Point &point) const override;
  std::vector<std::pair<double, const Shape *>>
  localIntersect(const Ray &ray) const override;
  ~Plane() = default;
};

class Cube : public Shape {
public:
  Cube() : Shape(){};
  Cube(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Vector localNormalAt(const Point &point) const override;
  std::vector<std::pair<double, const Shape *>>
  localIntersect(const Ray &ray) const override;
  ~Cube() = default;
};

using Intersection = std::pair<double, const Shape *>;

class Computations {
public:
  Computations(const Intersection &i, const Ray &r,
               const std::vector<Intersection> &xs = {});
  double t;
  double n1, n2;
  const Shape *object;
  Point point;
  Point overPoint;
  Point underPoint;
  Vector eye;
  Vector normal;
  Vector reflect;
  double schlick() const;
  bool inside;
};

template <typename... Intersections>
std::vector<Intersection> intersections(Intersections... is) {
  return std::vector<Intersection>{is...};
}

std::optional<Intersection> hit(const std::vector<Intersection> &xs);

Sphere glassSphere(Transformation transform = identityMatrix<4>(),
                   double transparency = 1.0, double refractiveIndex = 1.5);

} // namespace RT