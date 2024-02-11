#pragma once
#include "Light.hpp"
#include "Matrix.hpp"
#include "Pattern.hpp"
#include "Ray.hpp"
#include "Tuple.hpp"
#include "Util.hpp"
#include <limits>
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
  ~Material() = default;                     // Add destructor
  Material(Material &&m) noexcept = default; // Add move constructor
  Color color;
  double ambient;
  double diffuse;
  double specular;
  double shininess;
  double reflective;
  double transparency;
  double refractiveIndex;
  std::unique_ptr<Pattern> pattern;
  auto operator==(const Material &m) const -> bool;
  bool operator!=(const Material &m) const;
  Material &operator=(const Material &m);
  Material &operator=(Material &&m) noexcept;
};

class Shape {
public:
  Shape() : transformation(identityMatrix<4>()){};
  Shape(const Transformation &transformation, Material material)
      : transformation(transformation), material(std::move(material)){};
  Shape(const Shape &other) = default;
  Shape &operator=(const Shape &other) = default;
  Shape(Shape &&other) noexcept = default;
  Shape &operator=(Shape &&other) noexcept = default;
  Transformation transformation;
  Material material;
  [[nodiscard]] Tuple lighting(const Light &light, const Point &point,
                               const Vector &eye, const Vector &normal,
                               bool inShadow = false) const;
  [[nodiscard]] Color patternAt(const Point &point) const;
  [[nodiscard]] virtual Vector localNormalAt(const Point &point) const = 0;
  [[nodiscard]] Vector normalAt(const Point &point) const;
  [[nodiscard]] virtual std::vector<std::pair<double, const Shape *>>
  localIntersect(const Ray &ray) const = 0;
  [[nodiscard]] std::vector<std::pair<double, const Shape *>>
  intersect(const Ray &ray) const;
  virtual ~Shape() = default;
};

class Sphere : public Shape {
public:
  Sphere() = default;
  Sphere(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Sphere(const Sphere &other) = default;
  Sphere &operator=(const Sphere &other) = default;
  Sphere(Sphere &&other) noexcept = default;
  Sphere &operator=(Sphere &&other) noexcept = default;
  [[nodiscard]] Vector localNormalAt(const Point &point) const override;
  [[nodiscard]] std::vector<std::pair<double, const Shape *>>
  localIntersect(const Ray &ray) const override;
  ~Sphere() override = default;
};

class Plane : public Shape {
public:
  Plane() = default;
  Plane(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Plane(const Plane &other) = default;
  Plane &operator=(const Plane &other) = default;
  Plane(Plane &&other) noexcept = default;
  Plane &operator=(Plane &&other) noexcept = default;
  [[nodiscard]] Vector localNormalAt(const Point &point) const override;
  [[nodiscard]] std::vector<std::pair<double, const Shape *>>
  localIntersect(const Ray &ray) const override;
  ~Plane() override = default;
};

class Cube : public Shape {
public:
  Cube() = default;
  Cube(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Cube(const Cube &other) = default;
  Cube &operator=(const Cube &other) = default;
  Cube(Cube &&other) noexcept = default;
  Cube &operator=(Cube &&other) noexcept = default;
  [[nodiscard]] Vector localNormalAt(const Point &point) const override;
  [[nodiscard]] std::vector<std::pair<double, const Shape *>>
  localIntersect(const Ray &ray) const override;
  ~Cube() override = default;
};

class Cylinder : public Shape {
public:
  Cylinder()
      : minimum(-std::numeric_limits<double>::infinity()),
        maximum(std::numeric_limits<double>::infinity()), closed(false){};
  Cylinder(const Transformation &transformation, const Material &material,
           double min = -std::numeric_limits<double>::infinity(),
           double max = std::numeric_limits<double>::infinity(),
           bool closed = false)
      : Shape(transformation, material), minimum(min), maximum(max),
        closed(closed){};
  Cylinder(const Cylinder &other) = default;
  Cylinder &operator=(const Cylinder &other) = default;
  Cylinder(Cylinder &&other) noexcept = default;
  Cylinder &operator=(Cylinder &&other) noexcept = default;
  double minimum;
  double maximum;
  bool closed;
  [[nodiscard]] Vector localNormalAt(const Point &point) const override;
  [[nodiscard]] std::vector<std::pair<double, const Shape *>>
  localIntersect(const Ray &ray) const override;
  ~Cylinder() override = default;

private:
  [[nodiscard]] std::vector<std::pair<double, const Shape *>>
  intersectCaps(const Ray &ray) const;
  [[nodiscard]] static bool checkCap(const Ray &ray, double t);
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
  [[nodiscard]] double schlick() const;
  bool inside;
};

template <typename... Intersections>
std::vector<Intersection> intersections(Intersections... is) {
  return std::vector<Intersection>{is...};
}

constexpr double REFRACTIVE_INDEX_FOR_GLASS = 1.5;

std::optional<Intersection> hit(const std::vector<Intersection> &xs);

Sphere glassSphere(Transformation transform = identityMatrix<4>(),
                   double transparency = 1.0,
                   double refractiveIndex = REFRACTIVE_INDEX_FOR_GLASS);

} // namespace RT