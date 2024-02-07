#pragma once
#include "Matrix.hpp"
#include "Tuple.hpp"
namespace RT {

class Pattern {
public:
  Pattern() : transformation(identityMatrix<4>()){};
  virtual RT::Color patternAt(const RT::Point &p) const = 0;
  virtual std::unique_ptr<Pattern> clone() const = 0;
  virtual ~Pattern() = default;
  Transformation transformation;
};

class StripePattern : public Pattern {
public:
  Color a;
  Color b;
  StripePattern();
  StripePattern(const Color &a, const Color &b);
  Color patternAt(const Point &p) const override;
  std::unique_ptr<Pattern> clone() const override;
  ~StripePattern() = default;
};
} // namespace RT