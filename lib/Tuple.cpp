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

Tuple &Tuple::operator=(const Tuple &t) {
  this->x = t.x;
  this->y = t.y;
  this->z = t.z;
  this->w = t.w;
  return *this;
}

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
  return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

Tuple point(double x, double y, double z) { return Tuple(x, y, z, 1.0); }

Tuple vector(double x, double y, double z) { return Tuple(x, y, z, 0.0); }

Tuple color(double r, double g, double b) { return Tuple(r, g, b, 0.0); }

Tuple hadamard(const Tuple &a, const Tuple &b) {
  return Tuple(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}
double &Tuple::operator()(int i) {
  assert(i >= 0 && i < 4 && "out of bounds");
  return data[i];
}

const double &Tuple::operator()(int i) const {
  assert(i >= 0 && i < 4 && "out of bounds");
  return data[i];
}

Tuple Tuple::reflect(const Tuple &normal) const {
  return *this - normal * 2 * dot(*this, normal);
}

std::ostream &operator<<(std::ostream &os, const Tuple &t) {
  os << "Tuple(" << t.x << ", " << t.y << ", " << t.z << ", " << t.w << ")";
  return os;
}

} // namespace RT