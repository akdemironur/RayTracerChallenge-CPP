#pragma once
#include "Matrix.hpp"
#include "Tuple.hpp"
namespace RT {

class Pattern {
public:
  Pattern() : transformation(identityMatrix<4>()){};
  [[nodiscard]] virtual auto patternAt(const RT::Point &p) const
      -> RT::Color = 0;
  [[nodiscard]] virtual auto clone() const -> std::unique_ptr<Pattern> = 0;
  virtual ~Pattern() = default;
  Transformation transformation;
  Pattern(const Pattern &) = default;
  auto operator=(const Pattern &) -> Pattern & = default;
  Pattern(Pattern &&) = default;
  auto operator=(Pattern &&) -> Pattern & = default;
};

class StripePattern : public Pattern {
public:
  Color a;
  Color b;
  StripePattern();
  StripePattern(Color a, Color b);
  [[nodiscard]] auto patternAt(const Point &p) const -> Color override;
  [[nodiscard]] auto clone() const -> std::unique_ptr<Pattern> override;
  ~StripePattern() override = default;
  StripePattern(const StripePattern &) = default;
  auto operator=(const StripePattern &) -> StripePattern & = default;
  StripePattern(StripePattern &&) = default;
  auto operator=(StripePattern &&) -> StripePattern & = default;
};

class TestPattern : public Pattern {
public:
  TestPattern();
  [[nodiscard]] auto patternAt(const Point &p) const -> Color override;
  [[nodiscard]] auto clone() const -> std::unique_ptr<Pattern> override;
  ~TestPattern() override = default;
  TestPattern(const TestPattern &) = default;
  auto operator=(const TestPattern &) -> TestPattern & = default;
  TestPattern(TestPattern &&) = default;
  auto operator=(TestPattern &&) -> TestPattern & = default;
};

class GradientPattern : public Pattern {

public:
  Color a;
  Color b;
  GradientPattern();
  GradientPattern(Color a, Color b);
  [[nodiscard]] auto patternAt(const Point &p) const -> Color override;
  [[nodiscard]] auto clone() const -> std::unique_ptr<Pattern> override;
  ~GradientPattern() override = default;
  GradientPattern(const GradientPattern &) = default;
  auto operator=(const GradientPattern &) -> GradientPattern & = default;
  GradientPattern(GradientPattern &&) = default;
  auto operator=(GradientPattern &&) -> GradientPattern & = default;
};

class RingPattern : public Pattern {
public:
  Color a;
  Color b;
  RingPattern();
  RingPattern(Color a, Color b);
  [[nodiscard]] auto patternAt(const Point &p) const -> Color override;
  [[nodiscard]] auto clone() const -> std::unique_ptr<Pattern> override;
  ~RingPattern() override = default;
  RingPattern(const RingPattern &) = default;
  auto operator=(const RingPattern &) -> RingPattern & = default;
  RingPattern(RingPattern &&) = default;
  auto operator=(RingPattern &&) -> RingPattern & = default;
};

class CheckersPattern : public Pattern {
public:
  Color a;
  Color b;
  CheckersPattern();
  CheckersPattern(Color a, Color b);
  [[nodiscard]] auto patternAt(const Point &p) const -> Color override;
  [[nodiscard]] auto clone() const -> std::unique_ptr<Pattern> override;
  ~CheckersPattern() override = default;
  CheckersPattern(const CheckersPattern &) = default;
  auto operator=(const CheckersPattern &) -> CheckersPattern & = default;
  CheckersPattern(CheckersPattern &&) = default;
  auto operator=(CheckersPattern &&) -> CheckersPattern & = default;
};

} // namespace RT