#include "Tuple.hpp"
#include "Util.hpp"

namespace RT {

Tuple::Tuple() : x(0), y(0), z(0), w(0) {}

Tuple::Tuple(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

bool Tuple::isPoint() const { return w == 1.0; }

bool Tuple::isVector() const { return w == 0.0; }

Tuple Tuple::operator+(const Tuple &t) const {
  return Tuple(x + t.x, y + t.y, z + t.z, w + t.w);
}

Tuple Tuple::operator-(const Tuple &t) const {
  return Tuple(x - t.x, y - t.y, z - t.z, w - t.w);
}

Tuple Tuple::operator-() const { return Tuple(-x, -y, -z, -w); }

Tuple Tuple::operator*(const double &scalar) const {
  return Tuple(x * scalar, y * scalar, z * scalar, w * scalar);
}

Tuple operator*(const double &scalar, const Tuple &t) { return t * scalar; }

Tuple Tuple::operator/(const double &scalar) const {
  return Tuple(x / scalar, y / scalar, z / scalar, w / scalar);
}

bool Tuple::operator==(const Tuple &t) const {
  return isEqual(x, t.x) && isEqual(y, t.y) && isEqual(z, t.z) &&
         isEqual(w, t.w);
}

bool Tuple::operator!=(const Tuple &t) const { return !(*this == t); }

double Tuple::magnitude() const {
  return std::sqrt(x * x + y * y + z * z + w * w);
}
Tuple Tuple::norm() const { return *this / magnitude(); }

Tuple &Tuple::normalize() {
  auto mag = magnitude();
  this->x /= mag;
  this->y /= mag;
  this->z /= mag;
  this->w /= mag;
  return *this;
}

double dot(const Tuple &a, const Tuple &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Tuple cross(const Tuple &a, const Tuple &b) {
  return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

Tuple Point(double x, double y, double z) { return Tuple(x, y, z, 1.0); }

Tuple Vector(double x, double y, double z) { return Tuple(x, y, z, 0.0); }

Tuple Color(double r, double g, double b) { return Tuple(r, g, b, 0.0); }

Tuple hadamard(const Tuple &a, const Tuple &b) {
  return Tuple(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}
} // namespace RT