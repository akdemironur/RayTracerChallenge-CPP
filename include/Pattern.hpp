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

class TestPattern : public Pattern {
public:
  TestPattern();
  Color patternAt(const Point &p) const override;
  std::unique_ptr<Pattern> clone() const override;
  ~TestPattern() = default;
};

class GradientPattern : public Pattern {

public:
  Color a;
  Color b;
  GradientPattern();
  GradientPattern(const Color &a, const Color &b);
  Color patternAt(const Point &p) const override;
  std::unique_ptr<Pattern> clone() const override;
  ~GradientPattern() = default;
};

class RingPattern : public Pattern {
public:
  Color a;
  Color b;
  RingPattern();
  RingPattern(const Color &a, const Color &b);
  Color patternAt(const Point &p) const override;
  std::unique_ptr<Pattern> clone() const override;
  ~RingPattern() = default;
};

class CheckersPattern : public Pattern {
public:
  Color a;
  Color b;
  CheckersPattern();
  CheckersPattern(const Color &a, const Color &b);
  Color patternAt(const Point &p) const override;
  std::unique_ptr<Pattern> clone() const override;
  ~CheckersPattern() = default;
};

} // namespace RT