#pragma once
#include "Light.hpp"
#include "Matrix.hpp"
#include "Tuple.hpp"
#include "Util.hpp"
namespace RT {

class Material {
public:
  Material();
  Material(Color color, double ambient, double diffuse, double specular,
           double shininess);
  Color color;
  double ambient;
  double diffuse;
  double specular;
  double shininess;
  bool operator==(const Material &m) const;
  bool operator!=(const Material &m) const;
};

class Sphere {
public:
  Sphere();
  Sphere(const Transformation &transformation, const Material &material);
  bool operator==(const Sphere &s) const;
  bool operator!=(const Sphere &s) const;
  Vector normalAt(const Point &point) const;
  Transformation transformation;
  Material material;
};

Tuple lighting(const Material &material, const Light &light, const Point &point,
               const Vector &eye, const Vector &normal, bool inShadow = false);

} // namespace RT