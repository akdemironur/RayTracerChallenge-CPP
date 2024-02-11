#pragma once
#include "Matrix.hpp"
#include "Tuple.hpp"
namespace RT {

class Pattern {
public:
  Pattern() : transformation(identityMatrix<4>()){};
  [[nodiscard]] virtual RT::Color patternAt(const RT::Point &p) const = 0;
  [[nodiscard]] virtual std::unique_ptr<Pattern> clone() const = 0;
  virtual ~Pattern() = default;
  Transformation transformation;
  Pattern(const Pattern &) = default;
  Pattern &operator=(const Pattern &) = default;
  Pattern(Pattern &&) = default;
  Pattern &operator=(Pattern &&) = default;
};

class StripePattern : public Pattern {
public:
  Color a;
  Color b;
  StripePattern();
  StripePattern(Color a, Color b);
  [[nodiscard]] Color patternAt(const Point &p) const override;
  [[nodiscard]] std::unique_ptr<Pattern> clone() const override;
  ~StripePattern() override = default;
  StripePattern(const StripePattern &) = default;
  StripePattern &operator=(const StripePattern &) = default;
  StripePattern(StripePattern &&) = default;
  StripePattern &operator=(StripePattern &&) = default;
};

class TestPattern : public Pattern {
public:
  TestPattern();
  [[nodiscard]] Color patternAt(const Point &p) const override;
  [[nodiscard]] std::unique_ptr<Pattern> clone() const override;
  ~TestPattern() override = default;
  TestPattern(const TestPattern &) = default;
  TestPattern &operator=(const TestPattern &) = default;
  TestPattern(TestPattern &&) = default;
  TestPattern &operator=(TestPattern &&) = default;
};

class GradientPattern : public Pattern {

public:
  Color a;
  Color b;
  GradientPattern();
  GradientPattern(Color a, Color b);
  [[nodiscard]] Color patternAt(const Point &p) const override;
  [[nodiscard]] std::unique_ptr<Pattern> clone() const override;
  ~GradientPattern() override = default;
  GradientPattern(const GradientPattern &) = default;
  GradientPattern &operator=(const GradientPattern &) = default;
  GradientPattern(GradientPattern &&) = default;
  GradientPattern &operator=(GradientPattern &&) = default;
};

class RingPattern : public Pattern {
public:
  Color a;
  Color b;
  RingPattern();
  RingPattern(Color a, Color b);
  [[nodiscard]] Color patternAt(const Point &p) const override;
  [[nodiscard]] std::unique_ptr<Pattern> clone() const override;
  ~RingPattern() override = default;
  RingPattern(const RingPattern &) = default;
  RingPattern &operator=(const RingPattern &) = default;
  RingPattern(RingPattern &&) = default;
  RingPattern &operator=(RingPattern &&) = default;
};

class CheckersPattern : public Pattern {
public:
  Color a;
  Color b;
  CheckersPattern();
  CheckersPattern(Color a, Color b);
  [[nodiscard]] Color patternAt(const Point &p) const override;
  [[nodiscard]] std::unique_ptr<Pattern> clone() const override;
  ~CheckersPattern() override = default;
  CheckersPattern(const CheckersPattern &) = default;
  CheckersPattern &operator=(const CheckersPattern &) = default;
  CheckersPattern(CheckersPattern &&) = default;
  CheckersPattern &operator=(CheckersPattern &&) = default;
};

} // namespace RT