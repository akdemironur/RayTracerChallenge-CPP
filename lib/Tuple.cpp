#include "Tuple.hpp"
#include "Util.hpp"

namespace RT {

Tuple::Tuple()
    : data{0, 0, 0, 0}, x(data[0]), y(data[1]), z(data[2]), w(data[3]),
      red(data[0]), green(data[1]), blue(data[2]) {}

Tuple::Tuple(const Tuple &t)
    : data{t.x, t.y, t.z, t.w}, x(data[0]), y(data[1]), z(data[2]), w(data[3]),
      red(data[0]), green(data[1]), blue(data[2]) {}
Tuple::Tuple(double x, double y, double z, double w)
    : data{x, y, z, w}, x(data[0]), y(data[1]), z(data[2]), w(data[3]),
      red(data[0]), green(data[1]), blue(data[2]) {}

auto Tuple::operator=(Tuple &&t) noexcept -> Tuple & {
  this->x = t.x;
  this->y = t.y;
  this->z = t.z;
  this->w = t.w;
  return *this;
}

Tuple::Tuple(Tuple &&t) noexcept
    : data{t.x, t.y, t.z, t.w}, x(data[0]), y(data[1]), z(data[2]), w(data[3]),
      red(data[0]), green(data[1]), blue(data[2]){};

auto Tuple::isPoint() const -> bool { return w == 1.0; }

auto Tuple::isVector() const -> bool { return w == 0.0; }

auto Tuple::operator+(const Tuple &t) const -> Tuple {
  return {x + t.x, y + t.y, z + t.z, w + t.w};
}

auto Tuple::operator-(const Tuple &t) const -> Tuple {
  return {x - t.x, y - t.y, z - t.z, w - t.w};
}

auto Tuple::operator-() const -> Tuple { return {-x, -y, -z, -w}; }

auto Tuple::operator*(const double &scalar) const -> Tuple {
  return {x * scalar, y * scalar, z * scalar, w * scalar};
}

auto operator*(const double &scalar, const Tuple &t) -> Tuple {
  return t * scalar;
}

auto Tuple::operator/(const double &scalar) const -> Tuple {
  return {x / scalar, y / scalar, z / scalar, w / scalar};
}

auto Tuple::operator==(const Tuple &t) const -> bool {
  return approxEqual(x, t.x) && approxEqual(y, t.y) && approxEqual(z, t.z) &&
         approxEqual(w, t.w);
}

auto Tuple::operator!=(const Tuple &t) const -> bool { return !(*this == t); }

auto Tuple::operator=(const Tuple &t) -> Tuple & {
  if (this == &t) {
    return *this;
  }
  this->x = t.x;
  this->y = t.y;
  this->z = t.z;
  this->w = t.w;
  return *this;
}

auto Tuple::magnitude() const -> double {
  return std::sqrt(x * x + y * y + z * z + w * w);
}
auto Tuple::norm() const -> Tuple { return *this / magnitude(); }

auto Tuple::normalize() -> Tuple & {
  auto mag = magnitude();
  this->x /= mag;
  this->y /= mag;
  this->z /= mag;
  this->w /= mag;
  return *this;
}

auto dot(const Tuple &a, const Tuple &b) -> double {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

auto cross(const Tuple &a, const Tuple &b) -> Tuple {
  return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

auto point(double x, double y, double z) -> Tuple { return {x, y, z, 1.0}; }

auto vector(double x, double y, double z) -> Tuple { return {x, y, z, 0.0}; }

auto color(double r, double g, double b) -> Tuple { return {r, g, b, 0.0}; }

auto hadamard(const Tuple &a, const Tuple &b) -> Tuple {
  return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

auto Tuple::operator()(int i) -> double & {
  assert(i >= 0 && i < 4 && "out of bounds");
  return data.at(i);
}

auto Tuple::operator()(int i) const -> const double & {
  assert(i >= 0 && i < 4 && "out of bounds");
  return data.at(i);
}

auto Tuple::reflect(const Tuple &normal) const -> Tuple {
  return *this - normal * 2 * dot(*this, normal);
}

auto operator<<(std::ostream &os, const Tuple &t) -> std::ostream & {
  os << "Tuple(" << t.x << ", " << t.y << ", " << t.z << ", " << t.w << ")";
  return os;
}

} // namespace RT