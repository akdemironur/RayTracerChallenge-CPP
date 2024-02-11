#pragma once
#include "Util.hpp"
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
namespace RT {
class Tuple {
private:
  std::array<double, 4> data;

public:
  Tuple();
  Tuple(const Tuple &t);
  Tuple(Tuple &&t) noexcept;
  Tuple(double x, double y, double z, double w);
  double &x, &y, &z, &w;
  double &red, &green, &blue;
  [[nodiscard]] auto isPoint() const -> bool;
  [[nodiscard]] auto isVector() const -> bool;
  auto operator+(const Tuple &t) const -> Tuple;
  auto operator-(const Tuple &t) const -> Tuple;
  auto operator-() const -> Tuple;
  auto operator*(const double &scalar) const -> Tuple;
  auto operator/(const double &scalar) const -> Tuple;
  auto operator==(const Tuple &t) const -> bool;
  auto operator!=(const Tuple &t) const -> bool;
  friend auto operator<<(std::ostream &os, const Tuple &t) -> std::ostream &;
  auto operator=(const Tuple &t) -> Tuple &;
  auto operator=(Tuple &&t) noexcept -> Tuple &;
  auto operator()(int i) const -> const double &;
  auto operator()(int i) -> double &;
  [[nodiscard]] auto magnitude() const -> double;
  [[nodiscard]] auto reflect(const Tuple &normal) const -> Tuple;
  [[nodiscard]] auto norm() const -> Tuple;
  auto normalize() -> Tuple &;
  ~Tuple() = default;
};
auto point(double x, double y, double z) -> Tuple;
auto vector(double x, double y, double z) -> Tuple;
auto color(double r, double g, double b) -> Tuple;
auto dot(const Tuple &a, const Tuple &b) -> double;
auto cross(const Tuple &a, const Tuple &b) -> Tuple;
auto operator*(const double &scalar, const Tuple &t) -> Tuple;
auto hadamard(const Tuple &a, const Tuple &b) -> Tuple;
using Color = Tuple;
using Vector = Tuple;
using Point = Tuple;
} // namespace RT
