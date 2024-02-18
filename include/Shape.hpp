#pragma once
#include "Light.hpp"
#include "Matrix.hpp"
#include "Pattern.hpp"
#include "Ray.hpp"
#include "Tuple.hpp"
#include "Util.hpp"
#include <algorithm>
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
  auto operator!=(const Material &m) const -> bool;
  auto operator=(const Material &m) -> Material &;
  auto operator=(Material &&m) noexcept -> Material &;
};

class Shape {
public:
  Shape() : transformation(identityMatrix<4>()){};
  Shape(const Transformation &transformation, Material material)
      : transformation(transformation), material(std::move(material)){};
  Shape(const Shape &other) = default;
  auto operator=(const Shape &other) -> Shape & = default;
  Shape(Shape &&other) noexcept = default;
  auto operator=(Shape &&other) noexcept -> Shape & = default;
  Transformation transformation;
  Material material;
  [[nodiscard]] auto lighting(const Light &light, const Point &point,
                              const Vector &eye, const Vector &normal,
                              bool inShadow = false) const -> Tuple;
  [[nodiscard]] auto patternAt(const Point &point) const -> Color;
  [[nodiscard]] virtual auto localNormalAt(const Point &point) const
      -> Vector = 0;
  [[nodiscard]] auto normalAt(const Point &point) const -> Vector;
  [[nodiscard]] virtual auto localIntersect(const Ray &ray) const
      -> std::vector<std::pair<double, const Shape *>> = 0;
  [[nodiscard]] auto intersect(const Ray &ray) const
      -> std::vector<std::pair<double, const Shape *>>;
  virtual ~Shape() = default;
};

class Sphere : public Shape {
public:
  Sphere() = default;
  Sphere(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Sphere(const Sphere &other) = default;
  auto operator=(const Sphere &other) -> Sphere & = default;
  Sphere(Sphere &&other) noexcept = default;
  auto operator=(Sphere &&other) noexcept -> Sphere & = default;
  [[nodiscard]] auto localNormalAt(const Point &point) const -> Vector override;
  [[nodiscard]] auto localIntersect(const Ray &ray) const
      -> std::vector<std::pair<double, const Shape *>> override;
  ~Sphere() override = default;
};

class Plane : public Shape {
public:
  Plane() = default;
  Plane(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Plane(const Plane &other) = default;
  auto operator=(const Plane &other) -> Plane & = default;
  Plane(Plane &&other) noexcept = default;
  auto operator=(Plane &&other) noexcept -> Plane & = default;
  [[nodiscard]] auto localNormalAt(const Point &point) const -> Vector override;
  [[nodiscard]] auto localIntersect(const Ray &ray) const
      -> std::vector<std::pair<double, const Shape *>> override;
  ~Plane() override = default;
};

class Cube : public Shape {
public:
  Cube() = default;
  Cube(const Transformation &transformation, const Material &material)
      : Shape(transformation, material){};
  Cube(const Cube &other) = default;
  auto operator=(const Cube &other) -> Cube & = default;
  Cube(Cube &&other) noexcept = default;
  auto operator=(Cube &&other) noexcept -> Cube & = default;
  [[nodiscard]] auto localNormalAt(const Point &point) const -> Vector override;
  [[nodiscard]] auto localIntersect(const Ray &ray) const
      -> std::vector<std::pair<double, const Shape *>> override;
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
  auto operator=(const Cylinder &other) -> Cylinder & = default;
  Cylinder(Cylinder &&other) noexcept = default;
  auto operator=(Cylinder &&other) noexcept -> Cylinder & = default;
  double minimum;
  double maximum;
  bool closed;
  [[nodiscard]] auto localNormalAt(const Point &point) const -> Vector override;
  [[nodiscard]] auto localIntersect(const Ray &ray) const
      -> std::vector<std::pair<double, const Shape *>> override;
  ~Cylinder() override = default;

private:
  [[nodiscard]] auto intersectCaps(const Ray &ray) const
      -> std::vector<std::pair<double, const Shape *>>;
};

class Cone : public Shape {
public:
  Cone()
      : minimum(-std::numeric_limits<double>::infinity()),
        maximum(std::numeric_limits<double>::infinity()), closed(false){};
  Cone(const Transformation &transformation, const Material &material,
       double min = -std::numeric_limits<double>::infinity(),
       double max = std::numeric_limits<double>::infinity(),
       bool closed = false)
      : Shape(transformation, material), minimum(min), maximum(max),
        closed(closed){};
  Cone(const Cone &other) = default;
  auto operator=(const Cone &other) -> Cone & = default;
  Cone(Cone &&other) noexcept = default;
  auto operator=(Cone &&other) noexcept -> Cone & = default;
  double minimum;
  double maximum;
  bool closed;
  [[nodiscard]] auto localNormalAt(const Point &point) const -> Vector override;
  [[nodiscard]] auto localIntersect(const Ray &ray) const
      -> std::vector<std::pair<double, const Shape *>> override;

  ~Cone() override = default;

private:
  [[nodiscard]] auto intersectCaps(const Ray &ray) const
      -> std::vector<std::pair<double, const Shape *>>;
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
  [[nodiscard]] auto schlick() const -> double;
  bool inside;
};

template <typename... Intersections>
auto intersections(Intersections... is) -> std::vector<Intersection> {
  return std::vector<Intersection>{is...};
}

constexpr double REFRACTIVE_INDEX_FOR_GLASS = 1.5;

auto hit(const std::vector<Intersection> &xs) -> std::optional<Intersection>;

auto glassSphere(Transformation transform = identityMatrix<4>(),
                 double transparency = 1.0,
                 double refractiveIndex = REFRACTIVE_INDEX_FOR_GLASS) -> Sphere;

} // namespace RT
