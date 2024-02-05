#pragma once
#include "Util.hpp"
#include <cmath>
namespace RT {
class Tuple {
public:
  Tuple();
  Tuple(const Tuple &t);
  Tuple(double x, double y, double z, double w);
  double x, y, z, w;
  double &red, &green, &blue;
  bool isPoint() const;
  bool isVector() const;
  Tuple operator+(const Tuple &t) const;
  Tuple operator-(const Tuple &t) const;
  Tuple operator-() const;
  Tuple operator*(const double &scalar) const;
  Tuple operator/(const double &scalar) const;
  bool operator==(const Tuple &t) const;
  bool operator!=(const Tuple &t) const;
  Tuple &operator=(const Tuple &t);
  double magnitude() const;
  Tuple norm() const;
  Tuple &normalize();
};
Tuple point(double x, double y, double z);
Tuple vector(double x, double y, double z);
Tuple color(double r, double g, double b);
double dot(const Tuple &a, const Tuple &b);
Tuple cross(const Tuple &a, const Tuple &b);
Tuple operator*(const double &scalar, const Tuple &t);
Tuple hadamard(const Tuple &a, const Tuple &b);
using Color = Tuple;
using Vector = Tuple;
using Point = Tuple;
} // namespace RT
