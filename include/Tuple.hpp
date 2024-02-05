#pragma once
#include "Util.hpp"
#include <cmath>
namespace RT {
class Tuple {
public:
  Tuple();
  Tuple(double x, double y, double z, double w);
  double x, y, z, w;
  double &red = x, &green = y, &blue = z;
  bool isPoint() const;
  bool isVector() const;
  Tuple operator+(const Tuple &t) const;
  Tuple operator-(const Tuple &t) const;
  Tuple operator-() const;
  Tuple operator*(const double &scalar) const;
  Tuple operator/(const double &scalar) const;
  bool operator==(const Tuple &t) const;
  bool operator!=(const Tuple &t) const;
  double magnitude() const;
  Tuple norm() const;
  Tuple &normalize();
};
Tuple Point(double x, double y, double z);
Tuple Vector(double x, double y, double z);
Tuple Color(double r, double g, double b);
double dot(const Tuple &a, const Tuple &b);
Tuple cross(const Tuple &a, const Tuple &b);
Tuple operator*(const double &scalar, const Tuple &t);
Tuple hadamard(const Tuple &a, const Tuple &b);
} // namespace RT
