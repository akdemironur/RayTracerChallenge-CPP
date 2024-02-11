#include "Pattern.hpp"
#include <memory>

namespace RT {

StripePattern::StripePattern() : a(RT::color(1, 1, 1)), b(RT::color(0, 0, 0)){};
StripePattern::StripePattern(Color a, Color b)
    : a(std::move(a)), b(std::move(b)){};
auto StripePattern::clone() const -> std::unique_ptr<Pattern> {
  return std::make_unique<StripePattern>(*this);
};

auto StripePattern::patternAt(const Point &p) const -> Color {
  if (static_cast<int>(std::floor(p.x)) % 2 == 0) {
    return a;
  }
  return b;
}

TestPattern::TestPattern() = default;
;

auto TestPattern::patternAt(const Point &p) const -> Color {
  return RT::color(p.x, p.y, p.z);
}

auto TestPattern::clone() const -> std::unique_ptr<Pattern> {
  return std::make_unique<TestPattern>(*this);
}

GradientPattern::GradientPattern()
    : a(RT::color(1, 1, 1)), b(RT::color(0, 0, 0)){};

GradientPattern::GradientPattern(Color a, Color b)
    : a(std::move(a)), b(std::move(b)){};

auto GradientPattern::patternAt(const Point &p) const -> Color {
  auto distance = b - a;
  auto fraction = p.x - std::floor(p.x);
  return a + distance * fraction;
}

auto GradientPattern::clone() const -> std::unique_ptr<Pattern> {
  return std::make_unique<GradientPattern>(*this);
}

RingPattern::RingPattern() : a(RT::color(1, 1, 1)), b(RT::color(0, 0, 0)){};

RingPattern::RingPattern(Color a, Color b) : a(std::move(a)), b(std::move(b)){};

auto RingPattern::patternAt(const Point &p) const -> Color {
  if (int(std::floor(std::sqrt(p.x * p.x + p.z * p.z))) % 2 == 0) {
    return a;
  }
  return b;
}

auto RingPattern::clone() const -> std::unique_ptr<Pattern> {
  return std::make_unique<RingPattern>(*this);
}

CheckersPattern::CheckersPattern()
    : a(RT::color(1, 1, 1)), b(RT::color(0, 0, 0)){};

CheckersPattern::CheckersPattern(Color a, Color b)
    : a(std::move(a)), b(std::move(b)){};

auto CheckersPattern::patternAt(const Point &p) const -> Color {
  if ((int(std::floor(p.x)) + int(std::floor(p.y)) + int(std::floor(p.z))) %
          2 ==
      0) {
    return a;
  }
  return b;
}

auto CheckersPattern::clone() const -> std::unique_ptr<Pattern> {
  return std::make_unique<CheckersPattern>(*this);
}

} // namespace RT