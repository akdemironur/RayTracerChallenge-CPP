#include "Shape.hpp"
#include "Matrix.hpp"

namespace RT {

Material::Material()
    : color(RT::color(1, 1, 1)), ambient(0.1), diffuse(0.9), specular(0.9),
      shininess(200) {}

Material::Material(Color color, double ambient, double diffuse, double specular,
                   double shininess)
    : color(color), ambient(ambient), diffuse(diffuse), specular(specular),
      shininess(shininess) {}

bool Material::operator==(const Material &m) const {
  return color == m.color && isEqual(ambient, m.ambient) &&
         isEqual(diffuse, m.diffuse) && isEqual(specular, m.specular) &&
         isEqual(shininess, m.shininess);
}

bool Material::operator!=(const Material &m) const { return !(*this == m); }

Sphere::Sphere() : transformation(identityMatrix<4>()), material() {}

Sphere::Sphere(const Transformation &transformation, const Material &material)
    : transformation(transformation), material(material) {}

Vector Sphere::normalAt(const Point &p) const {
  auto objectPoint = transformation.inverse() * p;
  auto objectNormal = objectPoint - point(0, 0, 0);
  auto worldNormal = transformation.inverse().transpose() * objectNormal;
  worldNormal.w = 0;
  return worldNormal.norm();
}

Tuple lighting(const Material &material, const Light &light, const Point &point,
               const Vector &eye, const Vector &normal, bool inShadow) {
  auto effectiveColor = hadamard(material.color, light.intensity);
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

bool Sphere::operator==(const Sphere &s) const {
  return transformation == s.transformation && material == s.material;
}

bool Sphere::operator!=(const Sphere &s) const { return !(*this == s); }

} // namespace RT
