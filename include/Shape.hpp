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
           double shininess);
  Material(const Material &m);
  Color color;
  double ambient;
  double diffuse;
  double specular;
  double shininess;
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
  virtual Vector normalAt(const Point &point) const = 0;
  virtual std::vector<std::pair<double, const Shape *>>
  intersect(const Ray &ray) const = 0;
  virtual ~Shape() = default;
};

class Sphere : public Shape {
public:
  Sphere() : Shape(){};
  Sphere(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Vector normalAt(const Point &point) const override;
  std::vector<std::pair<double, const Shape *>>
  intersect(const Ray &ray) const override;
  ~Sphere() = default;
};

class Plane : public Shape {
public:
  Plane() : Shape(){};
  Plane(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Vector normalAt(const Point &point) const override;
  std::vector<std::pair<double, const Shape *>>
  intersect(const Ray &ray) const override;
  ~Plane() = default;
};

using Intersection = std::pair<double, const Shape *>;

class Computations {
public:
  Computations(const Intersection &i, const Ray &r);
  double t;
  const Shape *object;
  Point point;
  Point overPoint;
  Vector eye;
  Vector normal;
  bool inside;
};

template <typename... Intersections>
std::vector<Intersection> intersections(Intersections... is) {
  return std::vector<Intersection>{is...};
}

std::optional<Intersection> hit(const std::vector<Intersection> &xs);

} // namespace RT